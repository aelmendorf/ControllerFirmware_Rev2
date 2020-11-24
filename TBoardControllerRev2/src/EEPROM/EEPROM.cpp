/* 
* EEPROM.cpp
*
* Created: 9/26/2018 1:05:44 PM
* Author: 600076
*/


#include "EEPROM.h"

Recipe EEMEM eeprom_recipe;


// default constructor
EEPROM::EEPROM()
{
	
} //EEPROM

// default destructor
EEPROM::~EEPROM()
{
	
} //~EEPROM


void EEPROM::Initialize(){
	eeprom_read_block((void*)&this->recipe,(const void*)&eeprom_recipe,sizeof(this->recipe));
}

bool EEPROM::CheckInMemoryRecipe(){
	Recipe recipe;
	eeprom_read_block((void*)&recipe,(const void*)&eeprom_recipe,sizeof(recipe));
	return recipe==this->recipe;
}

void EEPROM::SaveRecipe(){
	eeprom_update_block((const void*)&this->recipe,(void*)&eeprom_recipe,sizeof(this->recipe));
}

void EEPROM::UpdateInMemoryLedRecipe(const LedRecipe &ledRecipe){
	this->recipe.SetLedRecipe(ledRecipe);
}

void EEPROM::UpdateInMemoryBoardRecipe(const Recipe &recipe){
	this->recipe=recipe;
}

/************************************************************************/
/*					Memory Access                                        */
/************************************************************************/

LedRecipe EEPROM::GetInMemoryLedRecipe(const LED &led){
	switch(led){
		case LED1:{
			return LedRecipe(led,this->recipe.Led1Delay,this->recipe.Led1RunTime,this->recipe.Led1Current);
		}
		case LED2:{
			return LedRecipe(led,this->recipe.Led2Delay,this->recipe.Led2RunTime,this->recipe.Led2Current);
		}
		case LED3:{
			return LedRecipe(led,this->recipe.Led3Delay,this->recipe.Led3RunTime,this->recipe.Led3Current);
		}
		default:{
			LedRecipe ledRecipe;
			ledRecipe.Led=NA;
			return ledRecipe;
		}
		
	}
}

LedRecipe EEPROM::GetStoredLedRecipe(const LED &led){
	switch(led){
		case LED1:{
			eeprom_read_block((void*)&this->recipe.Led1Delay,(const void*)&eeprom_recipe.Led1Delay,sizeof(this->recipe.Led1Delay));
			eeprom_read_block((void*)&this->recipe.Led1RunTime,(const void*)&eeprom_recipe.Led1RunTime,sizeof(this->recipe.Led1RunTime));
			eeprom_read_block((void*)&this->recipe.Led1Current,(const void*)&eeprom_recipe.Led1Current,sizeof(this->recipe.Led1Current));
			return LedRecipe(led,this->recipe.Led1Delay,this->recipe.Led1RunTime,this->recipe.Led1Current);
		}
		case LED2:{
			eeprom_read_block((void*)&this->recipe.Led2Delay,(const void*)&eeprom_recipe.Led2Delay,sizeof(this->recipe.Led2Delay));
			eeprom_read_block((void*)&this->recipe.Led2RunTime,(const void*)&eeprom_recipe.Led2RunTime,sizeof(this->recipe.Led2RunTime));
			eeprom_read_block((void*)&this->recipe.Led2Current,(const void*)&eeprom_recipe.Led2Current,sizeof(this->recipe.Led2Current));
			return LedRecipe(led,this->recipe.Led2Delay,this->recipe.Led2RunTime,this->recipe.Led2Current);
		}
		case LED3:{
			eeprom_read_block((void*)&this->recipe.Led3Delay,(const void*)&eeprom_recipe.Led3Delay,sizeof(this->recipe.Led3Delay));
			eeprom_read_block((void*)&this->recipe.Led3RunTime,(const void*)&eeprom_recipe.Led3RunTime,sizeof(this->recipe.Led3RunTime));
			eeprom_read_block((void*)&this->recipe.Led3Current,(const void*)&eeprom_recipe.Led3Current,sizeof(this->recipe.Led3Current));
			return LedRecipe(led,this->recipe.Led3Current,this->recipe.Led3RunTime,this->recipe.Led3Current);
		}
		default:{
			LedRecipe ledRecipe;
			ledRecipe.Led=NA;
			return ledRecipe;
		}
	}
}

Recipe EEPROM::GetStoredBoardRecipe(){
	Recipe recipe;
	eeprom_read_block((void*)&recipe,(const void*)&eeprom_recipe,sizeof(recipe));
	return recipe;
}

Recipe EEPROM::GetInMemoryBoardRecipe(){
	return this->recipe;
}