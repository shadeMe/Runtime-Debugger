#pragma once

namespace Legacy
{
	enum
	{
		kOpcode_BEGIN				= 0x10,
		kOpcode_END					= 0x11,
		kOpcode_SHORT_INT			= 0x12,
		kOpcode_LONG				= 0x13,
		kOpcode_FLOAT				= 0x14,
		kOpcode_SET					= 0x15,
		kOpcode_IF					= 0x16,
		kOpcode_ELSE				= 0x17,
		kOpcode_ELSEIF				= 0x18,
		kOpcode_ENDIF				= 0x19,
		kOpcode_WHILE_DEPRECATED	= 0x1A,
		kOpcode_ENDWHILE_DEPRECATED	= 0x1B,
		kOpcode_DOT					= 0x1C,
		kOpcode_SCRIPTNAME			= 0x1D,
		kOpcode_RETURN				= 0x1E,
		kOpcode_REFERENCE_REF		= 0x1F,
	};

	// 70C+?
	// represents a RPN encoded expression in a SET or IF/ElSEIF command
	class Expression
	{
	public:
		// 10
		struct Data10
		{
			// members
			/*00*/ UInt32			unk00;
			/*04*/ UInt32			unk04;
			/*08*/ UInt32			unk08;
			/*0C*/ UInt32			unk0C;
		};
		STATIC_ASSERT(sizeof(Data10) == 0x10);

		static const UInt32			kStackSize = 0x20;

		enum
		{
			kError_NoError			= 0,
			kError_OutOfMemory,
			kError_StackOverflow,
			kError_StackUnderflow,
			kError_DivideByZero,
			kError_Syntax,
			kError_BadObjectPointer
		};

		// members
		/*000*/ UInt32				errorCode;
		/*004*/ UInt32				unk004[(0x504 - 0x004) >> 2];
		/*504*/ SInt32				unk504;								// init to -1, stack depth?, probably corresponds to a stack at +0x304 
		/*508*/ Data10				unk508[kStackSize];
		/*708*/ SInt32				unk708;								// init to -1, stack depth?, corresponds to unk508
	};
	STATIC_ASSERT(sizeof(Expression) == 0x70C);

	// 14
	// represents an executing/active script's state
	class ScriptEventList
	{
	public:
		// OnActivate handled elsewhere (block handler looks up the ref's ExtraAction extradata)
		// OnAlarm, OnAlarmVictim are weird (multiple possible flags)
		enum
		{
			kEvent_OnAdd				= 0x00000001,
			kEvent_OnEquip				= 0x00000002,
			kEvent_OnActorEquip			= kEvent_OnEquip,	// presumably the game checks the type of the object
			kEvent_OnDrop				= 0x00000004,
			kEvent_OnUnequip			= 0x00000008,
			kEvent_OnActorUnequip		= kEvent_OnUnequip,
			kEvent_OnDeath				= 0x00000010,
			kEvent_OnMurder				= 0x00000020,
			kEvent_OnKnockout			= 0x00000040,
			kEvent_OnHit				= 0x00000080,
			kEvent_OnHitWith			= 0x00000100,		// TESObjectWEAP*
			kEvent_OnPackageStart		= 0x00000200,
			kEvent_OnPackageDone		= 0x00000400,
			kEvent_OnPackageChange		= 0x00000800,
			kEvent_OnLoad				= 0x00001000,
			kEvent_OnMagicEffectHit 	= 0x00002000,		// EffectSetting*
			kEvent_OnSell				= 0x00004000,
			kEvent_OnStartCombat		= 0x00008000,

			kEvent_OnAlarm_Steal		= 0x00010000,		// crime type 0
			kEvent_OnAlarm_Pickpocket	= 0x00020000,		// crime type 1
			kEvent_OnAlarm_Trespass 	= 0x00040000,		// crime type 2
			kEvent_OnAlarm_Attack		= 0x00080000,		// crime type 3
			kEvent_OnAlarm_Murder		= 0x00100000,		// crime type 4

			kEvent_OnTrigger			= 0x10000000,
			kEvent_OnTriggerActor		= 0x20000000,
			kEvent_OnTriggerMob			= 0x40000000,
			kEvent_OnReset				= 0x80000000
		};

		// 08
		struct Event
		{
			// members
			/*00*/ TESForm* 			object;
			/*04*/ UInt32				mask;
		};

		struct VariableEntry;

		// 18
		// this class is probably used by Script::VariableInfo (which just has an extra name member)
		struct Variable
		{
			enum
			{
				kType_FLOAT_REF			= 	0,		// same for both
				kType_INTEGER			= 	1,
			};

			// members
			/*00*/ UInt32				id;
			/*04*/ VarEntry* 			next;
			union
			{
				/*08*/ double			numeric;
				/*08*/ UInt32			formID;
			};
			/*10*/ UInt8				type;
			/*11*/ UInt8 				pad11[3];
			/*14*/ UInt32				unk14;
		};
		STATIC_ASSERT(sizeof(Variable) == 0x18);

		// 08
		struct VariableEntry
		{
			// members
			/*00*/ Variable* 			var;
			/*04*/ VariableEntry* 		next;
		};

		// 08	
		struct ScriptEffectInfo
		{
			// members
			/*00*/ UInt8				runEffectStartBlock;
			/*01*/ UInt8				runEffectFinishBlock;
			/*02*/ UInt16				pad02;
			/*04*/ float				elapsedSeconds;
		};

		typedef tList<Event>			EventListT;

		// members
		/*00*/ Script*					parent;
		/*04*/ UInt32					unk04;
		/*08*/ EventListT* 				events;
		/*0C*/ VariableEntry*			variables;
		/*10*/ ScriptEffectInfo*		scriptEffectInfo;
	};
	STATIC_ASSERT(sizeof(ScriptEventList) == 0x14);

	// A4
	// represents the currently executing script context
	class ScriptRunner
	{
	public:
		static const UInt32			kStackDepth = 10;

		enum
		{
			kStackFlags_IF			= 1 << 0,
			kStackFlags_ELSEIF		= 1 << 1,
			/* ELSE and ENDIF modify the above flags*/ 
		}

		// members
		/*00*/ TESObjectREFR*		containingObj;				// set when executing scripts on inventory objects
		/*04*/ TESForm*				callingRefBaseForm;
		/*08*/ ScriptEventList*		eventList;
		/*0C*/ UInt32				unk0C;
		/*10*/ UInt32				unk10;						// pointer? set to NULL before executing an instruction
		/*14*/ Script*				script;
		/*18*/ UInt32				unk18;						// set to 6 after a failed expression evaluation
		/*1C*/ UInt32				unk1C;						// set to Expression::errorCode
		/*20*/ UInt32				ifStackDepth;
		/*24*/ UInt32				ifStack[kStackDepth];		// stores flags
		/*4C*/ UInt32				unk4C[(0xA0 - 0x4C) >> 2];
		/*A0*/ UInt8				invalidReferences;			// set when the dot operator fails to resolve a reference (inside the error message handler)
		/*A1*/ UInt8				unkA1;						// set when the executing CommandInfo's 2nd flag bit (+0x25) is set
		/*A2*/ UInt16				padA2;
	};
	STATIC_ASSERT(sizeof(ScriptRunner) == 0xA4);


}