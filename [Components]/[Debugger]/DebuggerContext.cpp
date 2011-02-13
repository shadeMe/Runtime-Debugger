#include "DebuggerContext.h"
#include "Globals.h"
#include "[Common]\NativeWrapper.h"
#include "[Common]\ListViewUtilities.h"

DebuggerContext::DebuggerContext(Script* WorkingScript, ScriptEventList* WorkingEventList, StandardErrorOutput^ ErrorOutput, UInt16 CurrentOffset)
{
	TextViewer = gcnew ScriptDebuggerRTB(6, gcnew Font("Consolas", 9.75, FontStyle::Regular), Color::Black, Color::Gray, Color::Firebrick, Color::Yellow, Color::RosyBrown);
	
	MainSplitter = gcnew SplitContainer();
	ComponentToolbar = gcnew ToolStrip();
	ToolbarShowLocals = gcnew ToolStripButton();
	ToolbarShowGlobals = gcnew ToolStripButton();
	Locals = gcnew ListView();
	Globals = gcnew ListView();
	ValueEditBox = gcnew TextBox();

	SetupControlImage(ToolbarShowLocals);
	SetupControlImage(ToolbarShowGlobals);

	MainSplitter->Dock = DockStyle::Fill;
	MainSplitter->Orientation = Orientation::Horizontal;

	MainSplitter->Panel1->Controls->Add(TextViewer->GetContainer());
	MainSplitter->Panel2->Controls->Add(ComponentToolbar);
	MainSplitter->Panel2->Controls->Add(Locals);
	MainSplitter->Panel2->Controls->Add(Globals);
	MainSplitter->Panel2->Controls->Add(ValueEditBox);
	MainSplitter->SplitterDistance = 400;
	MainSplitter->BorderStyle = BorderStyle::FixedSingle;

	ValueEditBox->Font = gcnew Font("Consolas", 9.75, FontStyle::Regular);
	ValueEditBox->Multiline = true;
	ValueEditBox->BorderStyle = BorderStyle::FixedSingle;
	ValueEditBox->Visible = false;
	ValueEditBox->AcceptsReturn = true;
	ValueEditBox->LostFocus += gcnew EventHandler(this, &DebuggerContext::ValueEditBox_LostFocus);
	ValueEditBox->KeyDown += gcnew KeyEventHandler(this, &DebuggerContext::ValueEditBox_KeyDown);

	ComponentToolbar->AllowItemReorder = true;
	ComponentToolbar->AllowMerge = false;
	ComponentToolbar->Dock = System::Windows::Forms::DockStyle::Top;
	ComponentToolbar->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
	ComponentToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {ToolbarShowLocals, ToolbarShowGlobals});
	ComponentToolbar->RightToLeft = System::Windows::Forms::RightToLeft::Yes;

	ToolbarShowLocals->AutoSize = true;
	ToolbarShowLocals->Text = L"Show Script Locals";
	ToolbarShowLocals->ToolTipText = L"Show Script Locals";
	ToolbarShowLocals->Click += gcnew EventHandler(this, &DebuggerContext::ToolbarShowLocals_Click);
	ToolbarShowLocals->DisplayStyle = ToolStripItemDisplayStyle::Image;

	ToolbarShowGlobals->AutoSize = true;
	ToolbarShowGlobals->Text = L"Show Script Globals";
	ToolbarShowGlobals->ToolTipText = L"Show Script Globals";
	ToolbarShowGlobals->Click += gcnew EventHandler(this, &DebuggerContext::ToolbarShowGlobals_Click);
	ToolbarShowGlobals->DisplayStyle = ToolStripItemDisplayStyle::Image;

{
	ColumnHeader^ VariableName = gcnew ColumnHeader();
	VariableName->Text = "Variable Name";
	VariableName->Width = 250;
	ColumnHeader^ VariableValue = gcnew ColumnHeader();
	VariableValue->Text = "Variable Value";
	VariableValue->Width = 200;
	ColumnHeader^ VariableIndex = gcnew ColumnHeader();
	VariableIndex->Text = "Variable Index";
	VariableIndex->Width = 200;
	ColumnHeader^ VariableType = gcnew ColumnHeader();
	VariableType->Text = "Variable Type";
	VariableType->Width = 200;
	
	Locals->Font = gcnew Font("Consolas", 9.75, FontStyle::Regular);
	Locals->Dock = DockStyle::Fill;
	Locals->BorderStyle = BorderStyle::Fixed3D;
	Locals->BackColor = Color::White;
	Locals->ForeColor = Color::Black;
	Locals->View = View::Details;
	Locals->MultiSelect = false;
	Locals->CheckBoxes = false;
	Locals->FullRowSelect = true;
	Locals->HideSelection = false;
	Locals->SmallImageList = VariableIconList;
	Locals->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(4) {VariableName, VariableValue, VariableIndex, VariableType});
	Locals->DoubleClick += gcnew EventHandler(this, &DebuggerContext::AutosList_DoubleClick);
	Locals->Sorting = SortOrder::Ascending;
}

{
	ColumnHeader^ VariableName = gcnew ColumnHeader();
	VariableName->Text = "Variable Name";
	VariableName->Width = 250;
	ColumnHeader^ VariableValue = gcnew ColumnHeader();
	VariableValue->Text = "Variable Value";
	VariableValue->Width = 200;
	ColumnHeader^ VariableIndex = gcnew ColumnHeader();
	VariableIndex->Text = "Variable Index";
	VariableIndex->Width = 200;
	ColumnHeader^ VariableType = gcnew ColumnHeader();
	VariableType->Text = "Variable Type";
	VariableType->Width = 200;
	ColumnHeader^ ParentGlobalForm = gcnew ColumnHeader();

	Globals->Font = gcnew Font("Consolas", 9.75, FontStyle::Regular);
	Globals->Dock = DockStyle::Fill;
	Globals->BorderStyle = BorderStyle::Fixed3D;
	Globals->BackColor = Color::White;
	Globals->ForeColor = Color::Black;
	Globals->View = View::Details;
	Globals->ShowGroups = true;
	Globals->MultiSelect = false;
	Globals->CheckBoxes = false;
	Globals->FullRowSelect = true;
	Globals->HideSelection = false;
	Globals->SmallImageList = VariableIconList;
	Globals->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(4) {VariableName, VariableValue, VariableIndex, VariableType});
	Globals->DoubleClick += gcnew EventHandler(this, &DebuggerContext::AutosList_DoubleClick);
	Globals->Sorting = SortOrder::Ascending;
}

	this->WorkingScript = WorkingScript;
	this->WorkingEventList = WorkingEventList;
	this->ErrorOutput = ErrorOutput;

	this->CurrentOffset = CurrentOffset;
	State = kDebuggerState_Invalid;

	TextViewer->GetTextField()->Text = gcnew String(WorkingScript->text);
	TextViewer->GetTextField()->ReadOnly = true;

	if (TextViewer->CalculateLineOffsetsForTextField((UInt32)WorkingScript->data, WorkingScript->info.dataLength) == false)
		ErrorOutput("Couldn't calculate offsets for script!");

	UpdateUI();
	Locals->BringToFront();
}

String^ DebuggerContext::DescribeWorkingScript()
{ 
	return "Script " + WorkingScript->refID.ToString("x8") + " ~ " + "T[" + WorkingScript->info.type + "] ; V[" + WorkingScript->info.varCount + "] ; R[" + WorkingScript->info.numRefs + "]";
}

void DebuggerContext::UpdateExecutingOffset(UInt16 CurrentOffset)
{
	this->CurrentOffset = CurrentOffset;
}

void DebuggerContext::ParseEventListForEnumeration(Script* SourceScript, ScriptEventList* SourceEventList, ListView^% DestinationListView, ListViewGroup^ GlobalFormGroup)
{
	for (ScriptEventList::VarEntry* Itr = SourceEventList->m_vars; Itr != 0; Itr = Itr->next)
	{
		if (Itr->var == 0)
			continue;

		ListViewItem^ Item = gcnew ListViewItem();
		if (GlobalFormGroup != nullptr)
			Item->Group = GlobalFormGroup;

		Script::VariableInfo* VarInfo = SourceScript->GetVariableInfo(Itr->var->id);
		Script::RefVariable* RefInfo = SourceScript->GetVariable(Itr->var->id);
		if (VarInfo)
		{
			Item->Text = gcnew String(VarInfo->name.m_data);

			UInt32 RefData = *((UInt32*)&Itr->var->data);
			if (RefInfo)
				Item->SubItems->Add(RefData.ToString("x8"));
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

			Item->Tag = (UInt32)SourceEventList;
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
	ParseEventListForEnumeration(WorkingScript, WorkingEventList, Locals, nullptr);
	Locals->EndUpdate();

	Globals->BeginUpdate();

	Globals->Items->Clear();
	Globals->Groups->Clear();
	for (ScriptEventList::VarEntry* Itr = WorkingEventList->m_vars; Itr != 0; Itr = Itr->next)
	{
		if (Itr->var == 0)
			continue;

		Script::RefVariable* RefInfo = WorkingScript->GetVariable(Itr->var->id);
		if (RefInfo && (UInt32)Itr->var->data != 0x14)
		{
			UInt32 GlobalFormID = (UInt32)Itr->var->data;
			Script* GlobalScript = 0;
			ScriptEventList* GlobalEventList = 0;
			bool Result = false;
			NativeWrapper::GetScriptDataForForm(&Result, (UInt32)Itr->var->data, &GlobalScript, &GlobalEventList);

			if (Result == true)
			{
				ListViewGroup^ GlobalGroup = gcnew ListViewGroup(GlobalFormID.ToString("X8"), "Form " + GlobalFormID.ToString("X8")); 
				Globals->Groups->Add(GlobalGroup);
				ParseEventListForEnumeration(GlobalScript, GlobalEventList, Globals, GlobalGroup);
			}
		}
	}

	for (Script::RefListEntry* Itr = &WorkingScript->refList; Itr; Itr = Itr->next)
	{
		if (Itr->var == 0)
			continue;

		Script::RefVariable* GlobRef = Itr->var;
		if (GlobRef->form)
		{
			UInt32 GlobalFormID = GlobRef->form->refID;
			Script* GlobalScript = 0;
			ScriptEventList* GlobalEventList = 0;
			bool Result = false;
			NativeWrapper::GetScriptDataForForm(&Result, GlobalFormID, &GlobalScript, &GlobalEventList);

			if (Result == true)
			{
				ListViewGroup^ GlobalGroup = gcnew ListViewGroup(GlobalFormID.ToString("X8"), "Form " + GlobalFormID.ToString("X8")); 
				Globals->Groups->Add(GlobalGroup);
				ParseEventListForEnumeration(GlobalScript, GlobalEventList, Globals, GlobalGroup);
			}
		}
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
	return State;
}

void DebuggerContext::SetState(UInt8 State)
{
	this->State = State;
}

void DebuggerContext::AutosList_DoubleClick(Object^ Sender, EventArgs^ E)
{
	ListView^ ListViewSender = dynamic_cast<ListView^>(Sender);

	if (GetListViewSelectedItem(ListViewSender) != nullptr) {
		ListViewItem^ Item = GetListViewSelectedItem(ListViewSender);
		Rectangle Bounds = Item->SubItems[1]->Bounds;

		if (Bounds.Width > 35)
		{
			ValueEditBox->Tag = ListViewSender;
			ValueEditBox->SetBounds(Bounds.X + 2, Bounds.Y + Bounds.Height + 10, Bounds.Width, Bounds.Height, BoundsSpecified::All);
			ValueEditBox->Show();
			ValueEditBox->BringToFront();
			ValueEditBox->Focus();
		}
		else
		{
			MessageBox::Show("Please expand the Variable Value column sufficiently to allow the editing of its contents", "RUDE", 
							MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
	}		
}

void DebuggerContext::ValueEditBox_LostFocus(Object^ Sender, EventArgs^ E)
{
	ValueEditBox->Hide();
	if (ValueEditBox->Text == "")
		return;

	double Value = 0;
	try {
		Value = double::Parse(ValueEditBox->Text);
	} catch (...) {
		MessageBox::Show("Couldn't parse custom variable value", "RUDE", 
							MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		ValueEditBox->Text = "";
		return;
	}

	ValueEditBox->Text = "";
	ListView^ ListViewSender = dynamic_cast<ListView^>(ValueEditBox->Tag);

	if (GetListViewSelectedItem(ListViewSender) != nullptr)
	{
		ListViewItem^ Item = GetListViewSelectedItem(ListViewSender);
		ScriptEventList* SourceEventList = (ScriptEventList*)((UInt32)Item->Tag);
		UInt32 Index = UInt32::Parse(Item->SubItems[2]->Text);

		Item->SubItems[1]->Text = Value.ToString();
		SourceEventList->GetVariable(Index)->data = Value;
		ErrorOutput("Set the value of variable '" + Item->Text + "' to " + Value.ToString());
	}
}

void DebuggerContext::ValueEditBox_KeyDown(Object^ Sender, KeyEventArgs^ E)
{
	if (E->KeyCode == Keys::Enter)
		ValueEditBox_LostFocus(nullptr, nullptr);
}