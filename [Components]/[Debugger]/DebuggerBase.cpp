#include "DebuggerBase.h"
#include "[Common]\NativeWrapper.h"
#include "Globals.h"

DebuggerBase::DebuggerBase()
{
	DebuggerBox = gcnew Form();
	MainSplitter = (gcnew SplitContainer());
	CallStackSplitter = (gcnew SplitContainer());
	DebuggerToolbar = (gcnew ToolStrip());
	ToolbarQuitGame = gcnew ToolStripButton();
	ToolbarResumeExecution = gcnew ToolStripButton();
	ToolbarExecuteTillNextBreakpoint = gcnew ToolStripButton();
	ToolbarExecuteTillNextLine = gcnew ToolStripButton();
	ToolbarExecuteTillNextBlock = gcnew ToolStripButton();
	ToolbarExecuteTillNextError = gcnew ToolStripButton();
	ToolbarExecuteTillNextCommand = gcnew ToolStripButton();
	ToolbarExecuteTillReturn = gcnew ToolStripButton();
	ToolbarExecuteTillNextScript = gcnew ToolStripButton();
	Console = (gcnew ListView());
	CallStackList = (gcnew ListView());
	CallStackLabel = (gcnew ColumnHeader());

	SetupControlImage(ToolbarQuitGame);
	SetupControlImage(ToolbarResumeExecution);
	SetupControlImage(ToolbarExecuteTillNextBreakpoint);
	SetupControlImage(ToolbarExecuteTillNextLine);
	SetupControlImage(ToolbarExecuteTillNextBlock);
	SetupControlImage(ToolbarExecuteTillNextError);
	SetupControlImage(ToolbarExecuteTillNextCommand);
	SetupControlImage(ToolbarExecuteTillReturn);
	SetupControlImage(ToolbarExecuteTillNextScript);

	MainSplitter->Location = System::Drawing::Point(0, 26);
	MainSplitter->Name = L"MainSplitter";
	MainSplitter->Orientation = Orientation::Horizontal;
	MainSplitter->Dock = DockStyle::Fill;

	MainSplitter->Size = System::Drawing::Size(995, 532);
	MainSplitter->SplitterDistance = 408;
	MainSplitter->TabIndex = 0;
	MainSplitter->BorderStyle = BorderStyle::FixedSingle;

	CallStackSplitter->Dock = DockStyle::Fill;
	CallStackSplitter->Location = System::Drawing::Point(0, 20);
	CallStackSplitter->Name = L"CallStackSplitter";

	CallStackSplitter->Size = System::Drawing::Size(995, 408);
	CallStackSplitter->SplitterDistance = 331;
	CallStackSplitter->TabIndex = 0;
	CallStackSplitter->BorderStyle = BorderStyle::FixedSingle;

	DebuggerToolbar->AllowItemReorder = true;
	DebuggerToolbar->AllowMerge = false;
	DebuggerToolbar->Dock = DockStyle::Top;
	DebuggerToolbar->GripStyle = ToolStripGripStyle::Hidden;
	DebuggerToolbar->Location = System::Drawing::Point(0, 383);
	DebuggerToolbar->Name = L"DebuggerToolbar";
	DebuggerToolbar->RightToLeft = RightToLeft::Yes;
	DebuggerToolbar->Size = System::Drawing::Size(995, 25);
	DebuggerToolbar->TabIndex = 1;
	DebuggerToolbar->Text = L"Toolbar";

	SetupExecuteButton(ToolbarExecuteTillNextBreakpoint, "Execute Till Next Breakpoint", kDebuggerState_DebugTillNextBreakpoint);
	SetupExecuteButton(ToolbarExecuteTillNextLine, "Execute Till Next Line", kDebuggerState_DebugTillNextLine);
	SetupExecuteButton(ToolbarExecuteTillNextBlock, "Execute Till Next Block", kDebuggerState_DebugTillNextBlock);
	SetupExecuteButton(ToolbarExecuteTillNextError, "Execute Till Next Error", kDebuggerState_DebugTillNextError);
	SetupExecuteButton(ToolbarExecuteTillNextCommand, "Execute Till Next Command", kDebuggerState_DebugTillNextCommand);
	SetupExecuteButton(ToolbarExecuteTillReturn, "Execute Till Return", kDebuggerState_DebugTillReturn);
	SetupExecuteButton(ToolbarExecuteTillNextScript, "Execute Till Next Script", kDebuggerState_DebugTillNextScript);
	

	ToolbarResumeExecution->Text = L"Resume Execution";
	ToolbarResumeExecution->ToolTipText = L"Resume Execution";
	ToolbarResumeExecution->AutoSize = true;
	ToolbarResumeExecution->Click += gcnew EventHandler(this, &DebuggerBase::ToolbarResumeExecution_Click);

	ToolbarQuitGame->Text = L"Quit Runtime";
	ToolbarQuitGame->ToolTipText = L"Quit Runtime";
	ToolbarQuitGame->AutoSize = true;
	ToolbarQuitGame->Click += gcnew EventHandler(this, &DebuggerBase::ToolbarQuitGame_Click);
	
	DebuggerToolbar->Items->Add(ToolbarQuitGame);
	DebuggerToolbar->Items->Add(ToolbarResumeExecution);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillNextBreakpoint);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillNextLine);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillNextBlock);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillNextError);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillNextCommand);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillReturn);
	DebuggerToolbar->Items->Add(ToolbarExecuteTillNextScript);

	Console->Dock = System::Windows::Forms::DockStyle::Fill;
	Console->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
		static_cast<System::Byte>(0)));
	Console->HeaderStyle = ColumnHeaderStyle::Nonclickable;
	Console->HideSelection = false;
	Console->Location = System::Drawing::Point(0, 0);
	Console->MultiSelect = false;
	Console->Name = L"Console";
	Console->ShowGroups = false;
	Console->Size = System::Drawing::Size(331, 408);
	Console->TabIndex = 0;
	Console->UseCompatibleStateImageBehavior = false;
	Console->View = View::Details;
	
	ColumnHeader^ ConsoleLabel = gcnew ColumnHeader();
	ConsoleLabel->Text = L"Console";
	ConsoleLabel->Width = 900;

	Console->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(1) {ConsoleLabel});

	CallStackList->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(1) {CallStackLabel});
	CallStackList->Dock = DockStyle::Fill;
	CallStackList->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
		static_cast<System::Byte>(0)));
	CallStackList->HeaderStyle = ColumnHeaderStyle::Nonclickable;
	CallStackList->HideSelection = false;
	CallStackList->Location = System::Drawing::Point(0, 0);
	CallStackList->MultiSelect = false;
	CallStackList->Name = L"CallStack";
	CallStackList->ShowGroups = false;
	CallStackList->Size = System::Drawing::Size(331, 408);
	CallStackList->TabIndex = 0;
	CallStackList->UseCompatibleStateImageBehavior = false;
	CallStackList->View = View::Details;
	CallStackList->ItemActivate += gcnew EventHandler(this, &DebuggerBase::CallStackList_ItemActivate);

	CallStackLabel->Text = L"Call Stack";
	CallStackLabel->Width = 500;

	CallStackSplitter->Panel1->Controls->Add(CallStackList);
	MainSplitter->Panel1->Controls->Add(CallStackSplitter);
	MainSplitter->Panel2->Controls->Add(Console);


	DebuggerBox->Controls->Add(MainSplitter);
	DebuggerBox->Controls->Add(DebuggerToolbar);

	DebuggerBox->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	DebuggerBox->AutoScaleMode = AutoScaleMode::Font;
	DebuggerBox->ClientSize = System::Drawing::Size(1280, 780);
	DebuggerBox->ControlBox = false;
	DebuggerBox->Name = L"Interface";
	DebuggerBox->SizeGripStyle = SizeGripStyle::Show;
	DebuggerBox->StartPosition = FormStartPosition::CenterParent;
	DebuggerBox->Text = L"RUDE UI";
	DebuggerBox->FormClosing += gcnew FormClosingEventHandler(this, &DebuggerBase::DebuggerBox_Cancel);

	DebuggerBox->Hide();

	CallStack = gcnew Stack<DebuggerContext^>();
	Initialized = false;
}

DebuggerBase^% DebuggerBase::GetSingleton()
{
	if (Singleton == nullptr)
		Singleton = gcnew DebuggerBase();

	return Singleton;
}

DebuggerContext^ DebuggerBase::GetExecutingContext()
{
	if (CallStack->Count > 0)
		return CallStack->Peek();
	else
		return nullptr;
}

void DebuggerBase::SetupExecuteButton(ToolStripButton^ Button, String^ DisplayText, UInt8 State)
{
	Button->Text = DisplayText;
	Button->ToolTipText = DisplayText;
	Button->AutoSize = true;
	Button->Click += gcnew EventHandler(this, &DebuggerBase::ToolbarExecuteButtons_Click);
	Button->Tag = (int)State;
	Button->DisplayStyle = ToolStripItemDisplayStyle::Image;
}

void DebuggerBase::UpdateCallStackList()
{
	CallStackList->BeginUpdate();

	CallStackList->Items->Clear();
	for each (DebuggerContext^ Itr in CallStack)
	{
		ListViewItem^ Item = gcnew ListViewItem(Itr->DescribeWorkingScript());
		Item->Tag = Itr;
		CallStackList->Items->Add(Item);
	}

	CallStackList->EndUpdate();
}

void DebuggerBase::Initialize()
{
	Initialized = true;
}

void DebuggerBase::Deinitialize()
{
	Initialized = false;
	Console->Items->Clear();

	Hide(); 
}

void DebuggerBase::Show()
{
//	NativeWrapper::ReleaseCapture();
	DebuggerBox->ShowDialog();
}

UInt8 DebuggerBase::GetState()
{
	DebuggerContext^ CurrentContext = GetExecutingContext();
	if (CurrentContext)
		return CurrentContext->GetState();
	else
		return kDebuggerState_Break;
}

void DebuggerBase::SetState(UInt8 State)
{
	DebuggerContext^ CurrentContext = GetExecutingContext();
	if (CurrentContext)
		CurrentContext->SetState(State);

	if (State == kDebuggerState_Break)
		Show();
}

void DebuggerBase::DebugScript(Script* WorkingScript, ScriptEventList* EventList, UInt32 CurrentLine, UInt16 CurrentOffset)
{
	CallStack->Push(gcnew DebuggerContext(WorkingScript, EventList, gcnew StandardErrorOutput(this, &DebuggerBase::PrintToConsole), CurrentLine, CurrentOffset));
	UpdateCallStackList();

	DebuggerContext^% CurrentContext = GetExecutingContext();
	CallStackSplitter->Panel2->Controls->Add(CurrentContext->GetContainer());
	CurrentContext->GetContainer()->BringToFront();
}

UInt8 DebuggerBase::HandleScriptRunnerCallback(DebuggerMessage Message, UInt32* Data)
{
	DebuggerContext^ CurrentContext = GetExecutingContext();

	switch (Message)
	{
	case kDebuggerMessage_DebugBreakCalled:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];
			Script* WorkingScript = (Script*)Data[2];
			ScriptEventList* WorkingEventList = (ScriptEventList*)Data[3];

			if (GetIsInitialized() == 0)
			{
				DebugScript(WorkingScript, WorkingEventList, Line, Offset);
				Initialize();
			}
			else
				GetExecutingContext()->UpdateContext(Line, Offset);

			if (GetExecutingContext()->GetEventList() != WorkingEventList)
			{
				MessageBox::Show("ASSERTION FAILURE!");
				Debugger::Break();
			}

			
			PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Hit DebugBreak command...");
			SetState(kDebuggerState_Break);		
			break;
		}
	case kDebuggerMessage_NewInstance:
		{
			Script* WorkingScript = (Script*)Data[2];
			ScriptEventList* WorkingEventList = (ScriptEventList*)Data[3];

			DebugScript(WorkingScript, WorkingEventList, 0, 0);
			PrintToConsole("[0000]\t\t New ScriptRunner instance started - Script '" + WorkingScript->refID.ToString("x8") + "'");
			if (CurrentContext->GetState() == kDebuggerState_DebugTillNextScript)
			{
				CurrentContext->SetState(kDebuggerState_Break);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_NewLineExecutionStart:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

		//	PrintToConsole("[" + Offset.ToString("x4") + "]\t\t New line execution started");
			if (GetState() == kDebuggerState_DebugTillNextLine || (CurrentContext->GetIsOffsetBreakPoint(Offset) && GetState() == kDebuggerState_DebugTillNextBreakpoint))
			{
				if (CurrentContext->GetIsOffsetBreakPoint(Offset) && GetState() == kDebuggerState_DebugTillNextBreakpoint)
					PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Hit custom breakpoint...");
				GetExecutingContext()->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_ReferenceEvaluationFailed:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Reference evaluation failed - Current script blacklisted!");
			if (GetState() == kDebuggerState_DebugTillNextError)
			{
				GetExecutingContext()->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_EncounteredReturnCommand:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Hit Return command");
			if (GetState() == kDebuggerState_DebugTillReturn)
			{
				GetExecutingContext()->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_BlockHandlerCalled:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			const char* CommandName = (const char*)Data[2];
			PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Entering script block '" + gcnew String(CommandName) + "'...");	
			if (GetState() == kDebuggerState_DebugTillNextBlock)
			{
				GetExecutingContext()->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_CommandHandlerCalled:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			const char* CommandName = (const char*)Data[2];
			PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Executing script command '" + gcnew String(CommandName) + "'");				
			GetExecutingContext()->UpdateContext(Line, Offset);
			if (GetState() == kDebuggerState_DebugTillNextCommand)
			{
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_LineExecutionFailed:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			PrintToConsole("[" + Offset.ToString("x4") + "]\t\t Bytecode evaluation failed!");	
			if (GetState() == kDebuggerState_DebugTillNextError)
			{
				GetExecutingContext()->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_ExecutionComplete:
		{
			UInt32 Result = Data[0];
			DebuggerContext^ ExecutedContext = CallStack->Pop();
			CallStackSplitter->Panel2->Controls->Remove(ExecutedContext->GetContainer());
			UpdateCallStackList();

			PrintToConsole("[0000]\t\t Execution of script '" + CurrentContext->DescribeWorkingScript() + "' completed with result " + Result);	
			if (CallStack->Count == 0)
			{
				Deinitialize();
				return kDebuggerState_Invalid;
			}

			if (GetExecutingContext()->GetState() == kDebuggerState_Break)
				SetState(kDebuggerState_Break);
			break;
		}
	}
	return GetState();
}

void DebuggerBase::PrintToConsole(String^ Message)
{
	Console->Items->Add(Message);
	DebugPrint(Message);
	Console->TopItem = Console->Items[Console->Items->Count - 1];
}



void DebuggerBase::ToolbarQuitGame_Click(Object^ Sender, EventArgs^ E)
{
	NativeWrapper::ExitProcess(0);
}

void DebuggerBase::ToolbarResumeExecution_Click(Object^ Sender, EventArgs^ E)
{
	while (CallStack->Count)
		CallStackSplitter->Panel2->Controls->Remove(CallStack->Pop()->GetContainer());

	UpdateCallStackList();
	Deinitialize();
}

void DebuggerBase::ToolbarExecuteButtons_Click(Object^ Sender, EventArgs^ E)
{
	ToolStripButton^ Button = dynamic_cast<ToolStripButton^>(Sender);

	DebuggerState State = (DebuggerState)((int)Button->Tag);
	SetState(State);
	Hide();
}

void DebuggerBase::DebuggerBox_Cancel(Object^ Sender, FormClosingEventArgs^ E)
{
	E->Cancel = true;
}

void DebuggerBase::CallStackList_ItemActivate(Object^ Sender, EventArgs^ E)
{
	ListViewItem^ SelectedItem = CallStackList->SelectedItems[0];
	if (SelectedItem != nullptr)
	{
		DebuggerContext^ SelectedContext = dynamic_cast<DebuggerContext^>(SelectedItem->Tag);
		SelectedContext->GetContainer()->BringToFront();
	}
}


