#pragma once

enum DebuggerState
{
	kDebuggerState_Invalid = 0,
	kDebuggerState_Break,
	kDebuggerState_Debugging,

	kDebuggerState_DebugTillNextBreakpoint,
	kDebuggerState_DebugTillNextLine,
	kDebuggerState_DebugTillNextBlock,
	kDebuggerState_DebugTillNextError,
	kDebuggerState_DebugTillNextCommand,
	kDebuggerState_DebugTillReturn,
	kDebuggerState_DebugTillNextScript
};

enum DebuggerMessage
{																// Params: lineno and offset defaults
	kDebuggerMessage_DebugBreakCalled = 0,						//		script, eventlist
	kDebuggerMessage_NewInstance,								//		script, eventlist
	kDebuggerMessage_NewLineExecutionStart,
	kDebuggerMessage_ReferenceEvaluationFailed,					
	kDebuggerMessage_EncounteredReturnCommand,	
	kDebuggerMessage_BlockHandlerCalled,						//		block name
	kDebuggerMessage_CommandHandlerCalled,						//		command name	
	kDebuggerMessage_LineExecutionFailed,
	kDebuggerMessage_ExecutionComplete,							//		result
	kDebuggerMessage_OBSEScriptErrorEncountered					//		error message, command name
};

#ifndef OBLIVION		// ### figure out how to get the native sdk compile PROPERLY with clr projects
	class Script;

	class BaseFormComponent
	{
	public:
		BaseFormComponent();
		~BaseFormComponent();

		virtual void	Destructor(void);	// 00
		virtual void	Unk_01(void);
		virtual void	CopyFromBase(BaseFormComponent * component);
	};

	struct BSString
	{
		BSString();
		~BSString();

		char	* m_data;
		UInt16	m_dataLen;
		UInt16	m_bufLen;
	};

	struct ModEntry
	{
		// 41C / 420
		struct Data		// referred to as 'TESFile' by Bethesda
		{
			enum
			{
				kFlag_IsMaster =	1 << 0,
				kFlag_Loaded =		1 << 2,
				kFlag_Active =		1 << 3
			};

			template <typename tData> struct Node
			{
				tData		* data;
				Node<tData>	* next;
			};

			struct	ChunkInfo
			{
				UInt32	type;		// e.g. 'GRUP', 'GLOB', etc
				UInt32	length;
			};

			struct	FormInfo	// ###TODO double check this, see 46B910 (TESForm virtual func, accepts Unk23C* as arg)
			{
				ChunkInfo	chunkInfo;
				UInt32		flags;
				UInt32		formID;
				UInt32		unk10;
			};

			struct  SizeInfo	// as seen in the editor
			{
				UInt32		fileSizeLow;			// WIN32_FIND_DATA::nFileSizeLow 
				UInt32		fileSizeHigh;			// WIN32_FIND_DATA::nFileSizeHigh
			};

			// static members: B33C1C, B33C20

			UInt32	unk000;							// 000 appears to indicate status of file (open, closed, etc) 2, 9, 0C do stuff
			UInt32	unk004;							// 004
			UInt32	unk008;							// 008
			UInt32	unk00C;							// 00C
			void	* bsFile;						// 010
			UInt32	unk014;							// 014
			UInt32	unk018;							// 018
			char	name[0x104];					// 01C
			char	filepath[0x104];				// 120 relative to "Oblivion\"
			UInt32	unk224;							// 224
			UInt32	unk228;							// 228 init to *(0xB055CC), seen 0x2800
			UInt32	unk22C[(0x23C - 0x22C) >> 2];	// 22C
			FormInfo	formInfo;					// 23C
			UInt32	chunkType250;					// 250
			UInt32	unk254[(0x290 - 0x254) >> 2];	// 254
			UInt32	findData[(0x3D0 - 0x290) >> 2]; // 290 
			UInt32	version;						// 3D0 plugin version (0.8/1.0)
			UInt32	formCount;						// 3D4 record/form count
			UInt32	nextFormID;						// 3D8 used by TESFile::sub_486BF0 in the editor
			UInt32	flags;							// 3DC
			Node<char>		masterList;				// 3E0 linked list of .esm dependencies
			Node<SizeInfo>	masterSizeInfo;			// 3E8 linked list of file size info for above list
			UInt32	idx;							// 3F0
			void	* unk3F4;						// 3F4
			UInt32	unk3F8;							// 3F8
			UInt32	unk3FC;							// 3FC
			UInt8	unk400;							// 400 init to -1
			UInt8	pad401[3];
			BSString	authorName;					// 404
			BSString	modDescription;				// 40C
			UInt32	unk414;							// 414
			UInt32	unk418;							// 418
		};

		Data		* data;
		ModEntry	* next;

		ModEntry * Next() const	{	return next;	}
		Data * Info() const		{	return data;	}
		bool IsLoaded()	const	{	return (data && (data->flags & Data::kFlag_Loaded)) ? true : false;	}
	};

	class TESForm : public BaseFormComponent
	{
	public:
		TESForm();
		~TESForm();

		struct ModReferenceList
		{
			ModEntry			* data;
			ModReferenceList	* next;
		};

		UInt8	typeID;					// 004
		UInt8	typeIDPad[3];			// 005
		UInt32	flags;					// 008
		UInt32	refID;					// 00C
		ModReferenceList	modRefList;	// 010 / 01C
	};


	struct ScriptEventList
	{
		enum
		{
			kEvent_OnAdd			= 0x00000001,
			kEvent_OnEquip			= 0x00000002,
			kEvent_OnActorEquip		= kEvent_OnEquip,	// presumably the game checks the type of the object
			kEvent_OnDrop			= 0x00000004,
			kEvent_OnUnequip		= 0x00000008,
			kEvent_OnActorUnequip	= kEvent_OnUnequip,
			kEvent_OnDeath			= 0x00000010,
			kEvent_OnMurder			= 0x00000020,
			kEvent_OnKnockout		= 0x00000040,
			kEvent_OnHit			= 0x00000080,
			kEvent_OnHitWith		= 0x00000100,		// TESObjectWEAP*
			kEvent_OnPackageStart	= 0x00000200,
			kEvent_OnPackageDone	= 0x00000400,
			kEvent_OnPackageChange	= 0x00000800,
			kEvent_OnLoad			= 0x00001000,
			kEvent_OnMagicEffectHit = 0x00002000,		// EffectSetting*
			kEvent_OnSell			= 0x00004000,
			kEvent_OnStartCombat	= 0x00008000,

			kEvent_OnAlarm_Steal	= 0x00010000,		// crime type 0
			kEvent_OnAlarm_Pickpocket=0x00020000,		// crime type 1
			kEvent_OnAlarm_Trespass = 0x00040000,		// crime type 2
			kEvent_OnAlarm_Attack	= 0x00080000,		// crime type 3
			kEvent_OnAlarm_Murder	= 0x00100000,		// crime type 4

			kEvent_OnTrigger		= 0x10000000,
			kEvent_OnTriggerActor	= 0x20000000,
			kEvent_OnTriggerMob		= 0x40000000,
			kEvent_OnReset			= 0x80000000
		};

		struct Event
		{
			void	* object;		// TESForm*
			UInt32	eventMask;
		};

		struct EventEntry
		{
			Event		* event;
			EventEntry	* next;
		};

		struct VarEntry;

		struct Var
		{
			UInt32		id;
			VarEntry	* nextEntry;
			double		data;
		};

		struct VarEntry
		{
			Var			* var;
			VarEntry	* next;
		};

		struct ScriptEffectInfo
		{
			bool	bRunEffectStartBlock;
			bool	bRunEffectFinishBlock;
			UInt8	unk02[2];					// not seen accessed, probably pad
			float	elapsedSeconds;

			// may be more, some simple linked list
		};

		Script		* m_script;						// 00
		UInt32		m_unk1;							// 04
		EventEntry	* m_eventList;					// 08
		VarEntry	* m_vars;						// 0C
		ScriptEffectInfo	* m_scriptEffectInfo;	// 10

		Var *		GetVariable(UInt32 id)
		{
			for(VarEntry * entry = m_vars; entry; entry = entry->next)
				if(entry->var && entry->var->id == id)
					return entry->var;

			return 0;
		}
	};	

	class Script : public TESForm
	{
	public:
		Script();
		~Script();

		struct RefVariable
		{
			BSString	name;		// variable name/editorID (not used at run-time)
			TESForm	* form;		// TESForm*, non-zero for global refs
			UInt32	varIdx;		// always zero in editor
		};

		struct RefListEntry
		{
			RefVariable		* var;
			RefListEntry	* next;

			RefVariable* Info() const { return var; }
			RefListEntry* Next() const { return next; }
		};


		enum {
			eVarType_Float = 0,			//ref is also zero
			eVarType_Integer,

			// OBSE, return values only
			eVarType_String,
			eVarType_Array,
			eVarType_Ref,

			eVarType_Invalid
		};

		struct VariableInfo
		{
			UInt32			idx;		// 00
			UInt32			pad04;		// 04
			double			data;		// 08
			UInt8			type;		// 10
			UInt8			pad11[3];	// 11
			UInt32			unk14;		// 14
			BSString			name;		// 18
		};

		struct VarInfoEntry
		{
			VariableInfo	* data;
			VarInfoEntry	* next;

			VariableInfo* Info() const { return data; }
			VarInfoEntry* Next() const { return next; }
		};

		// 14
		struct ScriptInfo
		{
			UInt32	unk0;		// 00 
			UInt32	numRefs;	// 04 
			UInt32	dataLength;	// 08 
			UInt32	varCount;	// 0C 
			UInt32	type;		// 10 
		};

		enum {
			eType_Object = 0,
			eType_Quest = 1,
			eType_Magic = 0x100
		};

		ScriptInfo		info;					// 018 / 024
		char			* text;					// 02C / 038
		void			* data;					// 030 / 03C
		UInt32			unk34;					// 034		- time taken to execute script/total time spent executing script
		float			questDelayTimeCounter;	// 038      - init'd to fQuestDelayTime, decremented by frametime each frame
		float			secondsPassed;			// 03C      - only if you've modified fQuestDelayTime
		RefListEntry	refList;				// 040 / 040 - ref variables and immediates
		VarInfoEntry	varList;				// 048 / 048 - local variable list

		RefVariable	*	GetVariable(UInt32 reqIdx)
		{
			for(RefListEntry * entry = &refList; entry; entry = entry->next)
			{
				if(entry->var && entry->var->varIdx == reqIdx)
					return entry->var;
			}

			return 0;
		}
		VariableInfo*	GetVariableInfo(UInt32 idx)
		{
			for (Script::VarInfoEntry* entry = &varList; entry; entry = entry->next)
				if (entry->data && entry->data->idx == idx)
					return entry->data;

			return 0;
		}	
	};
#endif