#include "DebuggerContext.h"
#include "[Common]\NativeWrapper.h"

DebuggerContext::DebuggerContext(Script* WorkingScript, ScriptEventList* WorkingEventList, StandardErrorOutput^ ErrorOutput, UInt32 CurrentLine, UInt16 CurrentOffset)
{
	TextViewer = gcnew ScriptDebuggerRTB(6, gcnew Font("Consolas", 9.75, FontStyle::Regular), Color::Black, Color::White, Color::Blue, Color::Lime, Color::Yellow);
	
	MainSplitter = gcnew SplitContainer();
	ComponentToolbar = gcnew ToolStrip();
	ToolbarShowLocals = gcnew ToolStripButton();
	ToolbarShowGlobals = gcnew ToolStripButton();
	Locals = gcnew ListView();
	Globals = gcnew ListView();

	MainSplitter->Dock = DockStyle::Fill;
	MainSplitter->Orientation = Orientation::Horizontal;

	MainSplitter->Panel1->Controls->Add(TextViewer->GetContainer());
	MainSplitter->Panel2->Controls->Add(ComponentToolbar);
	MainSplitter->Panel2->Controls->Add(Locals);
	MainSplitter->Panel2->Controls->Add(Globals);
	MainSplitter->SplitterDistance = 400;

	ComponentToolbar->AllowItemReorder = true;
	ComponentToolbar->AllowMerge = false;
	ComponentToolbar->Dock = System::Windows::Forms::DockStyle::Top;
	ComponentToolbar->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
	ComponentToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {ToolbarShowLocals, ToolbarShowGlobals});
	ComponentToolbar->RightToLeft = System::Windows::Forms::RightToLeft::Yes;

	ToolbarShowLocals->ImageTransparentColor = System::Drawing::Color::Magenta;
	ToolbarShowLocals->Size = System::Drawing::Size(108, 22);
	ToolbarShowLocals->Text = L"Show Script Locals";
	ToolbarShowLocals->ToolTipText = L"Show Script Locals";
	ToolbarShowLocals->Click += gcnew EventHandler(this, &DebuggerContext::ToolbarShowLocals_Click);

	ToolbarShowGlobals->ImageTransparentColor = System::Drawing::Color::Magenta;
	ToolbarShowGlobals->Size = System::Drawing::Size(76, 22);
	ToolbarShowGlobals->Text = L"Show Script Globals";
	ToolbarShowGlobals->ToolTipText = L"Show Script Globals";
	ToolbarShowGlobals->Click += gcnew EventHandler(this, &DebuggerContext::ToolbarShowGlobals_Click);

	ColumnHeader^ VariableName = gcnew ColumnHeader();
	VariableName->Text = "Variable Name";
	VariableName->Width = 75;
	ColumnHeader^ VariableValue = gcnew ColumnHeader();
	VariableValue->Text = "Variable Value";
	VariableValue->Width = 100;
	ColumnHeader^ VariableIndex = gcnew ColumnHeader();
	VariableIndex->Text = "Variable Index";
	VariableIndex->Width = 100;
	ColumnHeader^ VariableType = gcnew ColumnHeader();
	VariableType->Text = "Variable Type";
	VariableType->Width = 100;
	ColumnHeader^ ParentGlobalForm = gcnew ColumnHeader();
	ParentGlobalForm->Text = "Global Form";
	ParentGlobalForm->Width = 75;


	Locals->Font = gcnew Font("Consolas", 9.75, FontStyle::Regular);
	Locals->Dock = DockStyle::Fill;
	Locals->BorderStyle = BorderStyle::Fixed3D;
	Locals->BackColor = Color::White;
	Locals->ForeColor = Color::Black;
	Locals->Visible = false;
	Locals->View = View::Details;
	Locals->MultiSelect = false;
	Locals->CheckBoxes = false;
	Locals->FullRowSelect = true;
	Locals->HideSelection = false;
	Locals->SmallImageList = VariableIconList;
	Locals->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(4) {VariableName, VariableValue, VariableIndex, VariableType});

	Globals->Font = gcnew Font("Consolas", 9.75, FontStyle::Regular);
	Globals->Dock = DockStyle::Fill;
	Globals->BorderStyle = BorderStyle::Fixed3D;
	Globals->BackColor = Color::White;
	Globals->ForeColor = Color::Black;
	Globals->Visible = false;
	Globals->View = View::Details;
	Globals->MultiSelect = false;
	Globals->CheckBoxes = false;
	Globals->FullRowSelect = true;
	Globals->HideSelection = false;
	Globals->SmallImageList = VariableIconList;
	Globals->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(5) {ParentGlobalForm, VariableName, VariableValue, VariableIndex, VariableType});

	this->WorkingScript = WorkingScript;
	this->WorkingEventList = WorkingEventList;
	this->ErrorOutput = ErrorOutput;

	this->CurrentLine = CurrentLine;
	this->CurrentOffset = CurrentOffset;
	State = gcnew ContextState();

	TextViewer->CalculateLineOffsetsForTextField((UInt32)WorkingScript->data, WorkingScript->info.dataLength);
	TextViewer->GetTextField()->Text = gcnew String(WorkingScript->text);

	UpdateContext(CurrentLine, CurrentOffset);
}

String^ DebuggerContext::DescribeWorkingScript()
{ 
	return WorkingScript->refID.ToString("x8"); 
}

void DebuggerContext::UpdateContext(UInt32 CurrentLine, UInt16 CurrentOffset)
{
	this->CurrentLine = CurrentLine;
	this->CurrentOffset = CurrentOffset;

	UpdateCurrentLine(CurrentLine);
	UpdateAutos();
}

void DebuggerContext::ParseEventListForEnumeration(Script* SourceScript, ScriptEventList* SourceEventList, ListView^% DestinationListView, UInt32 GlobalFormID)
{
	for (ScriptEventList::VarEntry* Itr = SourceEventList->m_vars; Itr != 0; Itr = Itr->next)
	{
		if (Itr->var == 0)
			continue;

		ListViewItem^ Item = gcnew ListViewItem();
		Script::VariableInfo* VarInfo = SourceScript->GetVariableInfo(Itr->var->id);
		Script::RefVariable* RefInfo = SourceScript->GetVariable(Itr->var->id);
		if (VarInfo)
		{
			if (GlobalFormID)
			{
				Item->Text = GlobalFormID.ToString("x8");
				Item->SubItems->Add(gcnew String(VarInfo->name.m_data));				
			}
			else
			{
				Item->Text = gcnew String(VarInfo->name.m_data);
			}

			if (RefInfo)
				Item->SubItems->Add(Itr->var->data.ToString("x8"));
			else
				Item->SubItems->Add(Itr->var->data.ToString());

			Item->SubItems->Add(Itr->var->id.ToString());

			switch (VarInfo->type)
			{
			case Script::eVarType_Float:
				if (RefInfo)
					Item->SubItems->Add("Reference");
				else
					Item->SubItems->Add("Float");
				break;
			default:
				Item->SubItems->Add("Integer");
				break;
			}

			DestinationListView->Items->Add(Item);
		}
		else
			ErrorOutput("Couldn't get variable info for index '" + Itr->var->id + "' in script " + SourceScript->refID.ToString("x8") + "!");
	}
}

void DebuggerContext::UpdateAutos()
{
	Locals->BeginUpdate();
	Locals->Items->Clear();
	ParseEventListForEnumeration(WorkingScript, WorkingEventList, Locals, 0);
	Locals->EndUpdate();

	Globals->BeginUpdate();

	Globals->Items->Clear();
	for (ScriptEventList::VarEntry* Itr = WorkingEventList->m_vars; Itr != 0; Itr = Itr->next)
	{
		if (Itr->var == 0)
			continue;

		Script::RefVariable* RefInfo = WorkingScript->GetVariable(Itr->var->id);
		if (RefInfo && (UInt32)Itr->var->data != 0x14)
		{
			Script* GlobalScript = 0;
			ScriptEventList* GlobalEventList = 0;

			if (NativeWrapper::GetScriptDataForForm((UInt32)Itr->var->data, &GlobalScript, &GlobalEventList))
			{
				ParseEventListForEnumeration(GlobalScript, GlobalEventList, Globals, (UInt32)Itr->var->data);
			}

			ListViewItem^ Item = gcnew ListViewItem("");
			Item->SubItems->Add(""), Item->SubItems->Add(""), Item->SubItems->Add(""), Item->SubItems->Add("");
			Globals->Items->Add(Item);
		}
		else
			ErrorOutput("Couldn't get variable info for index '" + Itr->var->id + "' in script " + WorkingScript->refID.ToString("x8") + "!");
	}

	Globals->EndUpdate();
}

void DebuggerContext::ToolbarShowLocals_Click(Object^ Sender, EventArgs^ E)
{
	Locals->BringToFront();
}

void DebuggerContext::ToolbarShowGlobals_Click(Object^ Sender, EventArgs^ E)
{
	Globals->BringToFront();
}

UInt8 DebuggerContext::GetState()
{
	UInt8 Result = kDebuggerState_Invalid;

	Monitor::Enter(State);
	Result = State->Get();
	Monitor::PulseAll(State);
	Monitor::Exit(State);

	return Result;
}

void DebuggerContext::SetState(UInt8 State)
{
	Monitor::Enter(this->State);
	this->State->Set(State);
	Monitor::PulseAll(this->State);
	Monitor::Exit(this->State);
}