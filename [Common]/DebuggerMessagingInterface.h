#pragma once

enum DebuggerState
{
	kDebuggerState_Invalid = 0,
	kDebuggerState_Break,
	kDebuggerState_Debugging,

	kDebuggerState_DebugTillNextLine,
	kDebuggerState_DebugTillNextBlock,
	kDebuggerState_DebugTillNextError,
	kDebuggerState_DebugTillNextCommand,
	kDebuggerState_DebugTillReturn
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
	kDebuggerMessage_ExecutionComplete							//		result
};

#ifndef OBLIVION		// ### figure out how to get the native sdk compile PROPERLY with clr projects
	class Script;

	class BaseFormComponent
	{
	public:
		BaseFormComponent();
		~BaseFormComponent();
	};

	struct BSString
	{
		BSString();
		~BSString();

		char	* m_data;
		UInt16	m_dataLen;
		UInt16	m_bufLen;
	};

	class TESForm : public BaseFormComponent
	{
	public:
		TESForm();
		~TESForm();

		struct ModReferenceList
		{
			void				* data;
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
			void	* form;		// TESForm*, non-zero for global refs
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
			UInt32	idx = 1;	// yes, really starts at 1
			for(RefListEntry * entry = &refList; entry; entry = entry->next)
			{
				if(idx == reqIdx)
					return entry->var;

				idx++;
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