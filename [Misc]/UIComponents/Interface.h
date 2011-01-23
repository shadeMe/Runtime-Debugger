#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Debugger {

	/// <summary>
	/// Summary for Interface
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Interface : public System::Windows::Forms::Form
	{
	public:
		Interface(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Interface()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  MainSplitter;
	private: System::Windows::Forms::SplitContainer^  CallStackSplitter;
	private: System::Windows::Forms::ToolStrip^  DebuggerToolbar;
	private: System::Windows::Forms::ToolStripButton^  ToolbarResumeDebugger;
	private: System::Windows::Forms::ToolStripButton^  ToolbarExecuteLine;
	private: System::Windows::Forms::TextBox^  Console;
	private: System::Windows::Forms::ListView^  CallStack;
	private: System::Windows::Forms::ColumnHeader^  CallStackLabel;










	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->MainSplitter = (gcnew System::Windows::Forms::SplitContainer());
			this->CallStackSplitter = (gcnew System::Windows::Forms::SplitContainer());
			this->DebuggerToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->ToolbarResumeDebugger = (gcnew System::Windows::Forms::ToolStripButton());
			this->ToolbarExecuteLine = (gcnew System::Windows::Forms::ToolStripButton());
			this->Console = (gcnew System::Windows::Forms::TextBox());
			this->CallStack = (gcnew System::Windows::Forms::ListView());
			this->CallStackLabel = (gcnew System::Windows::Forms::ColumnHeader());
			this->MainSplitter->Panel1->SuspendLayout();
			this->MainSplitter->Panel2->SuspendLayout();
			this->MainSplitter->SuspendLayout();
			this->CallStackSplitter->Panel1->SuspendLayout();
			this->CallStackSplitter->SuspendLayout();
			this->DebuggerToolbar->SuspendLayout();
			this->SuspendLayout();
			// 
			// MainSplitter
			// 
			this->MainSplitter->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MainSplitter->Location = System::Drawing::Point(0, 0);
			this->MainSplitter->Name = L"MainSplitter";
			this->MainSplitter->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// MainSplitter.Panel1
			// 
			this->MainSplitter->Panel1->Controls->Add(this->DebuggerToolbar);
			this->MainSplitter->Panel1->Controls->Add(this->CallStackSplitter);
			// 
			// MainSplitter.Panel2
			// 
			this->MainSplitter->Panel2->Controls->Add(this->Console);
			this->MainSplitter->Size = System::Drawing::Size(995, 532);
			this->MainSplitter->SplitterDistance = 408;
			this->MainSplitter->TabIndex = 0;
			// 
			// CallStackSplitter
			// 
			this->CallStackSplitter->Dock = System::Windows::Forms::DockStyle::Fill;
			this->CallStackSplitter->Location = System::Drawing::Point(0, 0);
			this->CallStackSplitter->Name = L"CallStackSplitter";
			// 
			// CallStackSplitter.Panel1
			// 
			this->CallStackSplitter->Panel1->Controls->Add(this->CallStack);
			this->CallStackSplitter->Size = System::Drawing::Size(995, 408);
			this->CallStackSplitter->SplitterDistance = 331;
			this->CallStackSplitter->TabIndex = 0;
			// 
			// DebuggerToolbar
			// 
			this->DebuggerToolbar->AllowItemReorder = true;
			this->DebuggerToolbar->AllowMerge = false;
			this->DebuggerToolbar->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->DebuggerToolbar->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->DebuggerToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ToolbarExecuteLine, 
				this->ToolbarResumeDebugger});
			this->DebuggerToolbar->Location = System::Drawing::Point(0, 383);
			this->DebuggerToolbar->Name = L"DebuggerToolbar";
			this->DebuggerToolbar->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->DebuggerToolbar->Size = System::Drawing::Size(995, 25);
			this->DebuggerToolbar->TabIndex = 1;
			this->DebuggerToolbar->Text = L"Toolbar";
			// 
			// ToolbarResumeDebugger
			// 
			this->ToolbarResumeDebugger->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->ToolbarResumeDebugger->Name = L"ToolbarResumeDebugger";
			this->ToolbarResumeDebugger->Size = System::Drawing::Size(108, 22);
			this->ToolbarResumeDebugger->Text = L"Resume Debugger";
			this->ToolbarResumeDebugger->ToolTipText = L"Resume Debugger";
			// 
			// ToolbarExecuteLine
			// 
			this->ToolbarExecuteLine->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->ToolbarExecuteLine->Name = L"ToolbarExecuteLine";
			this->ToolbarExecuteLine->Size = System::Drawing::Size(76, 22);
			this->ToolbarExecuteLine->Text = L"Execute Line";
			// 
			// Console
			// 
			this->Console->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->Console->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Console->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Console->Location = System::Drawing::Point(0, 0);
			this->Console->MaxLength = 65537;
			this->Console->Multiline = true;
			this->Console->Name = L"Console";
			this->Console->ReadOnly = true;
			this->Console->Size = System::Drawing::Size(995, 120);
			this->Console->TabIndex = 0;
			this->Console->TabStop = false;
			// 
			// CallStack
			// 
			this->CallStack->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->CallStackLabel});
			this->CallStack->Dock = System::Windows::Forms::DockStyle::Fill;
			this->CallStack->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->CallStack->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->CallStack->HideSelection = false;
			this->CallStack->Location = System::Drawing::Point(0, 0);
			this->CallStack->MultiSelect = false;
			this->CallStack->Name = L"CallStack";
			this->CallStack->ShowGroups = false;
			this->CallStack->Size = System::Drawing::Size(331, 408);
			this->CallStack->TabIndex = 0;
			this->CallStack->UseCompatibleStateImageBehavior = false;
			this->CallStack->View = System::Windows::Forms::View::Details;
			// 
			// CallStackLabel
			// 
			this->CallStackLabel->Text = L"Call Stack";
			this->CallStackLabel->Width = 200;
			// 
			// Interface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(995, 532);
			this->ControlBox = false;
			this->Controls->Add(this->MainSplitter);
			this->Name = L"Interface";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Show;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Script Debugger";
			this->MainSplitter->Panel1->ResumeLayout(false);
			this->MainSplitter->Panel1->PerformLayout();
			this->MainSplitter->Panel2->ResumeLayout(false);
			this->MainSplitter->Panel2->PerformLayout();
			this->MainSplitter->ResumeLayout(false);
			this->CallStackSplitter->Panel1->ResumeLayout(false);
			this->CallStackSplitter->ResumeLayout(false);
			this->DebuggerToolbar->ResumeLayout(false);
			this->DebuggerToolbar->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
