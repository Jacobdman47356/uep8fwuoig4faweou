/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2020  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GamepadPanel.h"
#include "../PAD.h"

GamepadPanel::GamepadPanel(wxNotebook* parent, unsigned int port, unsigned int slot)
	: wxPanel(parent)
{
	wxString title;
	m_port = port;
	m_slot = slot;
	GetPadName(title, m_port, m_slot);

	auto* tab_box = new wxBoxSizer(wxHORIZONTAL);
	auto* left_box = new wxBoxSizer(wxVERTICAL);

	pad_list = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(360, 400));
	pad_list->AppendTextColumn("Device", wxDATAVIEW_CELL_INERT, 120);
	pad_list->AppendTextColumn("PC", wxDATAVIEW_CELL_INERT, 120);
	pad_list->AppendTextColumn("PS2", wxDATAVIEW_CELL_INERT, 120);

	Update();
    left_box->Add(pad_list, wxSizerFlags().Expand());
	auto* right_box = new wxBoxSizer(wxVERTICAL);
	status_bar = new wxTextCtrl(this, wxID_ANY, "Gamepad Settings", wxDefaultPosition, wxSize(400, -1), wxTE_READONLY | wxTE_CENTRE);
	right_box->Add(status_bar, wxSizerFlags().Expand());

	auto* control_grid = new wxFlexGridSizer(2, 10, 10);

	auto* shoulder_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "Shoulder Buttons");
	auto* shoulder_grid = new wxFlexGridSizer(2, 5, 5);

	auto* l1_button = new wxButton(this, wxBTN_PAD_ID_L1, "L1");
	auto* r1_button = new wxButton(this, wxBTN_PAD_ID_R1, "R1");
	auto* l2_button = new wxButton(this, wxBTN_PAD_ID_L2, "L2");
	auto* r2_button = new wxButton(this, wxBTN_PAD_ID_R2, "R2");

	shoulder_grid->Add(l1_button);
	shoulder_grid->Add(r1_button);
	shoulder_grid->Add(l2_button);
	shoulder_grid->Add(r2_button);

	shoulder_box->Add(shoulder_grid);
	control_grid->Add(shoulder_box);

	auto* select_box = new wxStaticBoxSizer(wxHORIZONTAL, this);
	auto* select_grid = new wxFlexGridSizer(2, 5, 5);

	auto* select_button = new wxButton(this, wxBTN_PAD_ID_SELECT, "Select");
	auto* start_button = new wxButton(this, wxBTN_PAD_ID_START, "Start");
	auto* analog_button = new wxButton(this, wxBTN_PAD_ID_ANALOG, "Analog");
	auto* mouse_button = new wxButton(this, wxBTN_PAD_ID_MOUSE, "Mouse");
    analog_button->Disable();
    mouse_button->Disable();

	select_grid->Add(select_button);
	select_grid->Add(start_button);
	select_grid->Add(analog_button);
	select_grid->Add(mouse_button);

	select_box->Add(select_grid);
	control_grid->Add(select_box);

	auto* d_box = new wxStaticBoxSizer(wxVERTICAL, this, "D-Pad");
	auto* mid_d_box = new wxBoxSizer(wxHORIZONTAL);

	auto* up_button = new wxButton(this, wxBTN_PAD_ID_D_PAD_U, "Up");
	auto* left_button = new wxButton(this, wxBTN_PAD_ID_D_PAD_L, "Left");
	auto* right_button = new wxButton(this, wxBTN_PAD_ID_D_PAD_R, "Right");
	auto* down_button = new wxButton(this, wxBTN_PAD_ID_D_PAD_D, "Down");

	d_box->Add(up_button, wxSizerFlags().Centre());
	mid_d_box->Add(left_button);
	mid_d_box->Add(right_button);
	d_box->Add(mid_d_box);
	d_box->Add(down_button, wxSizerFlags().Centre());

	control_grid->Add(d_box);

	auto* face_box = new wxStaticBoxSizer(wxVERTICAL, this, "Face Buttons");
	auto* face_mid_box = new wxBoxSizer(wxHORIZONTAL);

	auto* triangle_button = new wxButton(this, wxBTN_PAD_ID_TRIANGLE, "Triangle");
	auto* square_button = new wxButton(this, wxBTN_PAD_ID_SQUARE, "Square");
	auto* circle_button = new wxButton(this, wxBTN_PAD_ID_CIRCLE, "Circle");
	auto* cross_button = new wxButton(this, wxBTN_PAD_ID_CROSS, "Cross");

	face_box->Add(triangle_button, wxSizerFlags().Centre());
	face_mid_box->Add(square_button);
	face_mid_box->Add(circle_button);
	face_box->Add(face_mid_box);
	face_box->Add(cross_button, wxSizerFlags().Centre());

	control_grid->Add(face_box);

	auto* l_stick_box = new wxStaticBoxSizer(wxVERTICAL, this, "Left Analog Stick");
	auto* l_stick_mid_box = new wxBoxSizer(wxHORIZONTAL);

	auto* l_stick_up_button = new wxButton(this, wxBTN_PAD_ID_L_STICK_U, "Up");
	auto* l_stick_left_button = new wxButton(this, wxBTN_PAD_ID_L_STICK_L, "Left");
	auto* l_stick_right_button = new wxButton(this, wxBTN_PAD_ID_L_STICK_R, "Right");
	auto* l_stick_down_button = new wxButton(this, wxBTN_PAD_ID_L_STICK_D, "Down");

	l_stick_box->Add(l_stick_up_button, wxSizerFlags().Centre());
	l_stick_mid_box->Add(l_stick_left_button);
	l_stick_mid_box->Add(l_stick_right_button);
	l_stick_box->Add(l_stick_mid_box);
	l_stick_box->Add(l_stick_down_button, wxSizerFlags().Centre());

	control_grid->Add(l_stick_box);

	auto* r_stick_box = new wxStaticBoxSizer(wxVERTICAL, this, "Left Analog Stick");
	auto* r_stick_mid_box = new wxBoxSizer(wxHORIZONTAL);

	auto* r_stick_up_button = new wxButton(this, wxBTN_PAD_ID_R_STICK_U, "Up");
	auto* r_stick_left_button = new wxButton(this, wxBTN_PAD_ID_R_STICK_L, "Left");
	auto* r_stick_right_button = new wxButton(this, wxBTN_PAD_ID_R_STICK_R, "Right");
	auto* r_stick_down_button = new wxButton(this, wxBTN_PAD_ID_R_STICK_D, "Down");

	r_stick_box->Add(r_stick_up_button, wxSizerFlags().Centre());
	r_stick_mid_box->Add(r_stick_left_button);
	r_stick_mid_box->Add(r_stick_right_button);
	r_stick_box->Add(r_stick_mid_box);
	r_stick_box->Add(r_stick_down_button, wxSizerFlags().Centre());

	control_grid->Add(r_stick_box);
	right_box->Add(control_grid, wxSizerFlags().Centre().Expand());

    auto* delete_button = new wxButton(this, wxBTN_PAD_ID_TRIANGLE, "Delete");
    auto* quick_setup_button = new wxButton(this, wxBTN_PAD_ID_TRIANGLE, "Quick Setup");
    delete_button->Disable();
    quick_setup_button->Disable();
	auto* button_box = new wxBoxSizer(wxHORIZONTAL);

    button_box->Add(delete_button);
    button_box->Add(quick_setup_button);
    left_box->Add(button_box, wxSizerFlags().Expand());

	tab_box->Add(left_box, wxSizerFlags().Expand());
	tab_box->Add(right_box, wxSizerFlags().Centre().Expand());

	SetSizerAndFit(tab_box);
	Bind(wxEVT_BUTTON, &GamepadPanel::ButtonPressed, this);
};

void GamepadPanel::Update()
{
	pad_list->DeleteAllItems();

	for (auto const& it : g_conf.keysym_map[m_port])
	{
		wxVector<wxVariant> data;

		//fprintf(stderr, "PAD %d:KEYSYM 0x%x = %d\n", m_port, it.first, it.second);
		data.push_back(wxVariant("Keyboard"));

		data.push_back(wxVariant(wxString(KeyName(m_port, 0, it.first))));
		data.push_back(wxVariant(wxString(pad_labels[it.second])));
		pad_list->AppendItem(data);
	}

	/*for(auto const& device : s_vgamePad)
    {
        fprintf(stderr, "Pulling\n");
        for(int i = 0; i < MAX_KEYS; i++)
        {
            wxVector<wxVariant> data;

            data.push_back(wxVariant(device->m_device_name));
            
            data.push_back(wxVariant(wxString(KeyName(m_port, 0, i))));
            data.push_back(wxVariant(wxString(pad_labels[i])));
            pad_list->AppendItem(data);
        }
    }*/
}

void GamepadPanel::CallUpdate(wxCommandEvent& event)
{
	Update();
}

void GamepadPanel::ButtonPressed(wxCommandEvent& event)
{
	wxButton* button = (wxButton*)event.GetEventObject(); // get the button object
	int button_id = button->GetId();

	if (btn_to_pad.count(button_id) > 0)
	{
		bool captured = false;
	    u32 key_pressed = 0;

		gamePadValues pad_key = btn_to_pad[button_id];

		status_bar->SetValue(wxString::Format("Press a key to set '%s' to, or Escape to cancel.", pad_labels[pad_key]));
        wxYieldIfNeeded();

		while (!captured)
		{
			if (PollForNewKeyboardKeys(key_pressed))
			{
				// special case for keyboard/mouse to handle multiple keys
				// Note: key_pressed == UINT32_MAX when ESC is hit to abort the capture
				if (key_pressed != UINT32_MAX)
				{
                    // Erase the keyboard binded key
                    u32 keysim = m_simulatedKeys[m_port][pad_key];
                    m_simulatedKeys[m_port][pad_key] = 0;

                    // erase gamepad entry (keysim map)
                    g_conf.keysym_map[pad_key].erase(keysim);

					g_conf.set_keyboard_key(m_port, key_pressed, pad_key);
					m_simulatedKeys[m_port][pad_key] = key_pressed;
				}
				captured = true;
			}
		}
        status_bar->SetValue(wxString("Gamepad Configuration."));
	}

	Update();
}

void GamepadPanel::Populate(int port, int slot, int padtype)
{
}