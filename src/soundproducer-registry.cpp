// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <winsock2.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "soundproducer-registry.h"


void SoundProducerRegistry::SetReferenceToSoundProducerVector(std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector)
{
	sound_producer_vector_ref = sound_producer_vector;
}

SoundProducer* SoundProducerRegistry::GetPointerToSoundProducerWithThisName(std::string thisName)
{
	//get iterator to sound producers vector from soundproducers map
	//std::unordered_map<std::string,std::vector <std::unique_ptr <SoundProducer> >::iterator>::const_iterator got = map_soundproducer.find (thisName);
	std::unordered_map<std::string,size_t>::const_iterator got = map_soundproducer.find (thisName);

	//std::vector <std::unique_ptr <SoundProducer> >::iterator it = got->second;
	size_t index = got->second;

	//return it->get();
	return sound_producer_vector_ref->at(index).get();
}

wxArrayString& SoundProducerRegistry::GetSoundProducersToEditList(){return soundproducers_to_edit_wxstring;}

void SoundProducerRegistry::AddRecentSoundProducerMadeToRegistry()
{
	if(sound_producer_vector_ref != nullptr)
	{
		//std::cout << "Add recent sound producer made to track called! \n";


		SoundProducer* thisSoundProducer = nullptr;
		//std::vector <std::unique_ptr <SoundProducer> >::iterator it;
		size_t index = 0;

		if(sound_producer_vector_ref->size() >= 1)
		{
			thisSoundProducer = sound_producer_vector_ref->at(sound_producer_vector_ref->size() - 1).get();
			//it = sound_producer_vector_ref->end() - 1;
			index = sound_producer_vector_ref->size() - 1;

			if(thisSoundProducer != nullptr)
			{
				wxString thisString(thisSoundProducer->GetNameString());

				soundproducers_to_edit_wxstring.Add(thisString);

				map_soundproducer.emplace(thisSoundProducer->GetNameString(),index);
			}
		}


	}

}

void SoundProducerRegistry::RemoveSoundProducerFromRegistry(SoundProducer* thisSoundProducer)
{
	wxString thisString(thisSoundProducer->GetNameString());
	soundproducers_to_edit_wxstring.Remove(thisString);

	//get index of sound producer to remove
	size_t index_to_rm  = 0;
	std::unordered_map<std::string,size_t>::const_iterator got = map_soundproducer.find (thisSoundProducer->GetNameString());
	index_to_rm = got->second;

	//decrement index of all soundproducers after this index
	for ( auto it = map_soundproducer.begin(); it != map_soundproducer.end(); ++it )
	{
		if(it->second > index_to_rm)
		{
			it->second--;
		}
	}

	//erase sound producer from map
	map_soundproducer.erase(thisSoundProducer->GetNameString());

}


void SoundProducerRegistry::RemoveThisNameFromEditingList(std::string thisName)
{
	soundproducers_to_edit_wxstring.Remove(thisName);
}


void SoundProducerRegistry::AddThisNameToEditingList(std::string thisName)
{
	soundproducers_to_edit_wxstring.Add(thisName);
}

void SoundProducerRegistry::AddAllSoundProducersToEditingList()
{
	if(sound_producer_vector_ref != nullptr)
	{
		soundproducers_to_edit_wxstring.Clear();
		for(size_t i=0; i < sound_producer_vector_ref->size(); i++)
		{
			wxString thisString(sound_producer_vector_ref->at(i)->GetNameString());

			soundproducers_to_edit_wxstring.Add(thisString);
		}
	}

}

void SoundProducerRegistry::AddReferenceToComboBox(wxComboBox* thisComboBox)
{
	combo_box_ptr_vec.push_back(thisComboBox);
}

void SoundProducerRegistry::RemoveLastComboBoxReference()
{
	combo_box_ptr_vec.pop_back();
}

void SoundProducerRegistry::UpdateAllComboBoxesList()
{
	for(size_t i=0; i < combo_box_ptr_vec.size(); i++)
	{
		wxString currentNameSelected = combo_box_ptr_vec[i]->GetStringSelection();

		combo_box_ptr_vec[i]->Clear();
		combo_box_ptr_vec[i]->Append(soundproducers_to_edit_wxstring);

		int select_index = combo_box_ptr_vec[i]->FindString(currentNameSelected,true); //case sensitive = true
		combo_box_ptr_vec[i]->SetSelection(select_index);
	}
}

void SoundProducerRegistry::RemoveThisNameFromAllComboBoxesExceptThisOne(std::string thisName, wxComboBox* thisComboBox)
{
	SoundProducerRegistry::AddAllSoundProducersToEditingList();

	SoundProducerRegistry::RemoveThisNameFromEditingList(thisName);

	for(size_t i=0; i < combo_box_ptr_vec.size(); i++)
	{
		if(combo_box_ptr_vec[i] != thisComboBox)
		{
			wxString currentSelectionString = combo_box_ptr_vec[i]->GetStringSelection();

			combo_box_ptr_vec[i]->Clear();
			combo_box_ptr_vec[i]->Append(soundproducers_to_edit_wxstring);

			int select_index = combo_box_ptr_vec[i]->FindString(currentSelectionString,true); //case sensitive = true
			combo_box_ptr_vec[i]->SetSelection(select_index);
		}
	}
}


