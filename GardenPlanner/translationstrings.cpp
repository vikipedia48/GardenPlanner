#include "translationstrings.h"

Util::Language Translate::currentLanguage = Util::Language::ENGLISH;
int Translate::lang = 0;

const tr Translate::_gardenNotSelected = {"Garden not selected", "Vrt nije odabran"};
const tr Translate::_youMustSelectOneActivity = {"You must select one activity", "Morate odabrati jednu aktivnost"};
const tr Translate::_youMustSelectAtLeastOnePlant = {"You must select at least one plant", "Morate odabrati makar jednu biljku"};
const tr Translate::_areYouSureYouWantToDeleteThis = {"Are you sure you want to delete this?", "Jeste li sigurni da želite izbrisati ovo?"};
const tr Translate::_yourUsernameMustBeLonger = {"Your username must be longer","Vaše korisničko ime mora biti dulje"};
const tr Translate::_passwordsDontMatch = {"Passwords don't match","Lozinke se ne podudaraju"};
const tr Translate::_errorInCreatingNewAccount = {"Error in creating new account","Pogreška pri stvaranju novog korisničkog računa"};
const tr Translate::_success = {"Success", "Uspjeh"};
const tr Translate::_nameCannotBeEmpty = {"Name cannot be empty","Ime ne smije biti prazno"};
const tr Translate::_passwordCannotBeEmpty = {"Password cannot be empty","Lozinka ne smije biti prazna"};
const tr Translate::_wrongUsernameSlashPassword = {"Wrong username/password","Pogrešno korisničko ime ili lozinka"};
const tr Translate::_areYouSureYouWantToExit = {"Are you sure you want to exit?","Jeste li sigurni da želite izaći?"};
const tr Translate::_areYouSureYouWantToSaveChanges = {"Are you sure you want to save changes?","Jeste li sigurni da želite spremiti promjene?"};
const tr Translate::_nameInRow = {"Name in row ","Ime u retku "};
const tr Translate::_idInRow = {"ID in row ","ID u retku "};
const tr Translate::_cannotBeEmpty = {" cannot be empty"," ne smije biti prazno"};
const tr Translate::_mustBeUnique = {" must be unique"," mora biti jedinstven"};
const tr Translate::_pleaseEnterAUniqueNumber = {". Please enter a unique number.",". Molim vas unesite jedinstven broj"};
const tr Translate::_errorReadingFile = {"Error reading file","Pogreška pri čitanju datoteke"};
const tr Translate::_errorReadingJson = {"Error reading JSON","Pogreška pri čitanju JSON datoteke"};
const tr Translate::_areYouSureYouWantToClearTheTable = {"Are you sure you want to clear the table?","Jeste li sigurni da želite očistiti tablicu?"};
const tr Translate::_doYouWantToLoadPreviousSettings = {"Do you want to load previous settings?","Želite li učitati prijašnje stavke?"};
const tr Translate::_pleaseNameYourGarden = {"Please name your garden?", "Molim vas, imenujte vrt"};
const tr Translate::_youMustSelectActivitiesToRemove = {"You must select activities to remove", "Morate odabrati aktivnosti koje želite maknuti"};
const tr Translate::_pleaseInputTheNewWorkersName = {"Please input the new worker's name","Molim vas unesite ime novoga radnika"};
const tr Translate::_repeatMustBeEmptyOrNonNegative = {"Repeat must be empty or a non-negative number","Ponavljanje mora biti prazno ili nenegativan broj"};
const tr Translate::_durationMustBeEmptyOrNonNegative = {"Duration must be empty or a non-negative number","Trajanje mora biti prazno ili nenegativan broj"};
const tr Translate::_errorGettingResource = {"Error getting resource", "Greška pri dohvaćanju resursa"};
const tr Translate::_error = {"Error", "Greška"};
const tr Translate::_invalidInput = {"Invalid input", "Pogrešan unos"};
const tr Translate::_question = {"Question", "Pitanje"};
const tr Translate::_enterYear = {"Enter year", "Unesite godinu"};
const tr Translate::_enterMonth = {"Enter month", "Unesite mjesec"};
const tr Translate::_january = {"January","Siječanj"};
const tr Translate::_february = {"February","Veljača"};
const tr Translate::_march = {"March","Ožujak"};
const tr Translate::_april = {"April","Travanj"};
const tr Translate::_may = {"May","Svibanj"};
const tr Translate::_june = {"June","Lipanj"};
const tr Translate::_july = {"July","Srpanj"};
const tr Translate::_august = {"August","Kolovoz"};
const tr Translate::_september = {"September","Rujan"};
const tr Translate::_october = {"October","Listopad"};
const tr Translate::_november = {"November","Studeni"};
const tr Translate::_december = {"December","Prosinac"};
const tr Translate::_pdfDocuments = {"PDF documents (*.pdf)","PDF dokumenti (*.pdf)"};
const tr Translate::_saveFile = {"Save file","Spremite datoteku"};
const tr Translate::_repeatDays = {"Repeat (days)","Ponavljanje (u danima)"};
const tr Translate::_plant = {"Plant","Biljka"};
const tr Translate::_activitiesScheduledForThisDate = {"Activities scheduled for this date","Aktivnosti planirane za ovaj dan"};
const tr Translate::_newWorker = {"New worker", "Novi radnik"};
const tr Translate::_pleaseWait = {"Please wait", "Pričekajte"};
const tr Translate::_yourFileHasChanged = {"Your custom plants file has changed. Some plants might not be valid if they are no longer present in the file.", "Vaša datoteka s korisničkim biljkama promijenila se. Neke biljke možda neće biti ispravne ako više nisu u datoteci."};
const tr Translate::_clearCache = {"Clear cache","Očisti cache"};
const tr Translate::_youMustLoadActivities = {"You must Load this garden's activities before doing this. If you have already done that, ignore this message.","Morate Učitati aktivnosti prije ovoga. Ako ste već učinili to, zanemarite ovu poruku."};
const tr Translate::_load = {"Load", "Učitaj"};
const tr Translate::_noActivities = {"No activities", "Nema aktivnosti"};
const tr Translate::_creditCardNumber = {"Credit card number","Broj kreditne kartice"};
const tr Translate::_expirationDate = {"Expiration date","Datum isteka kartice"};
const tr Translate::_securityCode = {"Security code","PIN"};
const tr Translate::_invalidSerialKey = {"Failed to validate purchase", "Neuspješno validiranje kupnje"};
const tr Translate::_purchaseGardenPlanner = {"Purchase GardenPlanner", "Kupite GardenPlanner"};

const tr Translate::_configIniNotFound = {"config.ini not found, reverting to default settings...", "config.ini nije pronađen, povratak na originalne postavke..."};
const tr Translate::_invalidFontSize = {"Invalid font size", "Neispravna veličina teksta"};
const tr Translate::_areYouSureYouWantToRevert = {"Are you sure you want to revert to default settings?", "Jeste li sigurni da se želite vratiti na originalne postavke?"};
const tr Translate::_activities = {"Activities","Aktivnosti"};
const tr Translate::_add = {"Add","Dodaj"};
const tr Translate::_addActivity = {"Add activity","Dodaj aktivnost"};
const tr Translate::_addWorkerToActivity = {"Add worker to activity","Odredi radnika za aktivnost"};
const tr Translate::_assignedWorkers = {"Assigned workers","Dodijeljeni radnici"};
const tr Translate::_addGarden = {"Add garden","Dodaj vrt"};
const tr Translate::_createGarden = {"Create garden","Napravi vrt"};
const tr Translate::_deleteGarden = {"Delete garden","Izbriši vrt"};
const tr Translate::_delete_ = {"Delete","Izbriši"};
const tr Translate::_editGarden = {"Edit garden","Uredi vrt"};
const tr Translate::_gardenName = {"Garden name","Ime vrta"};
const tr Translate::_gardens = {"Gardens","Vrtovi"};
const tr Translate::_viewGardens = {"View gardens","Prikaži vrtove"};
const tr Translate::_viewCalendar = {"View calendar","Prikaži kalendar"};
const tr Translate::_viewSlashEdit = {"View/Edit","Prikaži/Uredi"};
const tr Translate::_viewing = {"Viewing","Prikazuje se"};
const tr Translate::_apply = {"Apply","Primijeni"};
const tr Translate::_cancel = {"Cancel","Odbaci"};
const tr Translate::_confirmPassword = {"Confirm password","Ponovi lozinku"};
const tr Translate::_password = {"Password","Lozinka"};
const tr Translate::_customPlants = {"Custom plants","Korisničke biljke"};
const tr Translate::_dark = {"Dark","Tamni"};
const tr Translate::_date = {"Date","Datum"};
const tr Translate::_defaultSettings = {"Default settings","Originalne postavke"};
const tr Translate::_description = {"Description","Opis"};
const tr Translate::_durationDays = {"Duration (days)","Trajanje (u danima)"};
const tr Translate::_repeatEveryXDays = {"Repeat every X days","Ponovi svakih X dana"};
const tr Translate::_fontSize = {"Font size","Veličina teksta"};
const tr Translate::_goHome = {"Go home","Odi na početak"};
const tr Translate::_home = {"Home","Početak"};
const tr Translate::_language = {"Language","Jezik"};
const tr Translate::_login = {"Login","Prijava"};
const tr Translate::_logout = {"Logout","Odjava"};
const tr Translate::_makeMonthlyReport = {"Make monthly report","Napravi mjesečni izvještaj"};
const tr Translate::_region = {"Region","Regija"};
const tr Translate::_name = {"Name","Ime"};
const tr Translate::_register_ = {"Register","Registracija"};
const tr Translate::_save = {"Save","Spremi"};
const tr Translate::_remove = {"Remove","Makni"};
const tr Translate::_select = {"Select","Odaberi"};
const tr Translate::_settings = {"Settings","Postavke"};
const tr Translate::_theme = {"Theme","Izgled"};
const tr Translate::_username = {"Username","Korisničko ime"};
const tr Translate::_white = {"White","Svijetli"};
const tr Translate::_welcome = {"Welcome, ","Dobrodošli, "};
const tr Translate::_youAreNotLoggedIn = {"You are not logged in.","Niste prijavljeni."};
const tr Translate::_user = {"User","Korisnik"};
const tr Translate::_clear = {"Clear","Očisti"};

const std::array<std::string,2> Translate::_plant1 = {"Chard","Blitva"};
const std::array<std::string,2> Translate::_plant2 = {"Cabbage","Kupusnjače"};
const std::array<std::string,2> Translate::_plant3 = {"Carrot","Mrkva"};
const std::array<std::string,2> Translate::_plant4 = {"Radish","Rotkvica"};
const std::array<std::string,2> Translate::_plant5 = {"Black radish","Crna rotkva"};
const std::array<std::string,2> Translate::_plant6 = {"Dill","Kopar"};
const std::array<std::string,2> Translate::_plant7 = {"Cucumber","Krastavac"};
const std::array<std::string,2> Translate::_plant8 = {"Chamomile","Kamilica"};
const std::array<std::string,2> Translate::_plant9 = {"Garlic","Češnjak"};
const std::array<std::string,2> Translate::_plant10 = {"Kohlrabi","Korabica"};
const std::array<std::string,2> Translate::_plant11 = {"Leek","Poriluk"};
const std::array<std::string,2> Translate::_plant12 = {"Tomato","Rajčica"};
const std::array<std::string,2> Translate::_plant13 = {"Coriander","Korijandar"};
const std::array<std::string,2> Translate::_plant14 = {"Caraway","Kim"};
const std::array<std::string,2> Translate::_plant15 = {"Leaf lettuce","Lisnata salata"};
const std::array<std::string,2> Translate::_plant16 = {"Zucchini","Tikvice"};
const std::array<std::string,2> Translate::_plant17 = {"Onion","Luk"};
const std::array<std::string,2> Translate::_plant18 = {"Head lettuce","Salata glavatica"};
const std::array<std::string,2> Translate::_plant19 = {"Strawberry","Jagoda"};
const std::array<std::string,2> Translate::_plant20 = {"Raspberry","Malina"};
const std::array<std::string,2> Translate::_plant21 = {"Lillies","Ljiljani"};
const std::array<std::string,2> Translate::_plant22 = {"Fruits","Voćke"};
const std::array<std::string,2> Translate::_plant23 = {"Roses","Ruže"};
const std::array<std::string,2> Translate::_plant24 = {"Beet","Cikla"};
const std::array<std::string,2> Translate::_plant25 = {"Tulip","Tulipan"};
const std::array<std::string,2> Translate::_plant26 = {"Fennel","Komorač"};
const std::array<std::string,2> Translate::_plant27 = {"Beans","Grah"};
const std::array<std::string,2> Translate::_plant28 = {"Savory","Čubar"};
const std::array<std::string,2> Translate::_plant29 = {"Potato","Krumpir"};
const std::array<std::string,2> Translate::_plant30 = {"Celery","Celer"};
const std::array<std::string,2> Translate::_plant31 = {"Corn","Kukuruz"};
const std::array<std::string,2> Translate::_plant32 = {"Trachystemon ","Oštrolistac"};
const std::array<std::string,2> Translate::_plant33 = {"Chive","Vlasac"};
const std::array<std::string,2> Translate::_plant34 = {"Spinach","Špinat"};
const std::array<std::string,2> Translate::_plant35 = {"Endive","Endivija"};
const std::array<std::string,2> Translate::_plant36 = {"Peas","Grašak"};
const std::array<std::string,2> Translate::_plant37 = {"Corn salad","Matovilac"};
const std::array<std::string,2> Translate::_plant38 = {"Sage","Kadulja"};
const std::array<std::string,2> Translate::_plant39 = {"Chicory","Radič"};
const std::array<std::string,2> Translate::_plant40 = {"Asparagus","Šparoga"};
const std::array<std::string,2> Translate::_plant41 = {"Black salsify","Crni korijen"};
const std::array<std::string,2> Translate::_plant42 = {"Fava bean","Bob"};
const std::array<std::string,2> Translate::_plant43 = {"Nasturtium","Dragoljub"};
const std::array<std::string,2> Translate::_plant44 = {"Horseradish","Hren"};
const std::array<std::string,2> Translate::_plant45 = {"Peppermint","Paprena metvica"};
const std::array<std::string,2> Translate::_plant46 = {"Marigold","Kadifica"};
const std::array<std::string,2> Translate::_plant47 = {"Pumpkins","Bundeve"};
const std::array<std::string,2> Translate::_plant48 = {"Sunflower","Suncokret"};
const std::array<std::string,2> Translate::_plant49 = {"Watermelon","Lubenica"};
const std::array<std::string,2> Translate::_plant50 = {"Wormwood","Pravi pelin"};
const std::array<std::string,2> Translate::_plant51 = {"Mustard","Gorušica"};
const std::array<std::string,2> Translate::_plant52 = {"Rosemary","Ružmarin"};
const std::array<std::string,2> Translate::_plant53 = {"Garden cress","Grbica"};
const std::array<std::string,2> Translate::_plant54 = {"Chervil","Krasuljica"};
const std::array<std::string,2> Translate::_plant55 = {"Parsley","Peršin"};
const std::array<std::string,2> Translate::_plant56 = {"Cauliflower","Cvjetača"};
const std::array<std::string,2> Translate::_plant57 = {"Pepper","Paprika"};

void Translate::setLanguage(Util::Language language)
{
    currentLanguage = language;
    lang = language == Util::Language::CROATIAN ? 1 : 0;
}

QString Translate::gardenNotSelected() {return _gardenNotSelected[lang]; }
QString Translate::youMustSelectOneActivity() {return _youMustSelectOneActivity[lang]; }
QString Translate::youMustSelectAtLeastOnePlant() {return _youMustSelectAtLeastOnePlant[lang]; }
QString Translate::areYouSureYouWantToDeleteThis() {return _areYouSureYouWantToDeleteThis[lang]; }
QString Translate::configIniNotFound() {return _configIniNotFound[lang]; }
QString Translate::invalidFontSize() {return _invalidFontSize[lang]; }
QString Translate::areYouSureYouWantToRevert() {return _areYouSureYouWantToRevert[lang]; }
QString Translate::nameCannotBeEmpty() {return _nameCannotBeEmpty[lang]; }
QString Translate::passwordCannotBeEmpty() {return _passwordCannotBeEmpty[lang]; }
QString Translate::wrongUsernameSlashPassword() {return _wrongUsernameSlashPassword[lang]; }
QString Translate::areYouSureYouWantToExit() {return _areYouSureYouWantToExit[lang]; }
QString Translate::areYouSureYouWantToSaveChanges() {return _areYouSureYouWantToSaveChanges[lang]; }
QString Translate::nameInRow() {return _nameInRow[lang]; }
QString Translate::idInRow() {return _idInRow[lang]; }
QString Translate::cannotBeEmpty() {return _cannotBeEmpty[lang]; }
QString Translate::mustBeUnique() {return _mustBeUnique[lang]; }
QString Translate::pleaseEnterAUniqueNumber() {return _pleaseEnterAUniqueNumber[lang]; }
QString Translate::errorReadingFile() {return _errorReadingFile[lang]; }
QString Translate::errorReadingJson() {return _errorReadingJson[lang]; }
QString Translate::areYouSureYouWantToClearTheTable() {return _areYouSureYouWantToClearTheTable[lang]; }
QString Translate::doYouWantToLoadPreviousSettings() {return _doYouWantToLoadPreviousSettings[lang]; }
QString Translate::pleaseNameYourGarden() {return _pleaseNameYourGarden[lang]; }
QString Translate::youMustSelectActivitiesToRemove() {return _youMustSelectActivitiesToRemove[lang]; }
QString Translate::pleaseInputTheNewWorkersName() {return _pleaseInputTheNewWorkersName[lang]; }
QString Translate::repeatMustBeEmptyOrNonNegative() {return _repeatMustBeEmptyOrNonNegative[lang]; }
QString Translate::durationMustBeEmptyOrNonNegative() {return _durationMustBeEmptyOrNonNegative[lang]; }
QString Translate::yourUsernameMustBeLonger() {return _yourUsernameMustBeLonger[lang]; }
QString Translate::errorInCreatingNewAccount() {return _errorInCreatingNewAccount[lang]; }
QString Translate::passwordsDontMatch() {return _passwordsDontMatch[lang]; }
QString Translate::success() {return _success[lang]; }
QString Translate::errorGettingResource() {return _errorGettingResource[lang]; }
QString Translate::error() {return _error[lang]; }
QString Translate::invalidInput() {return _invalidInput[lang]; }
QString Translate::question() {return _question[lang]; }
QString Translate::enterYear() {return _enterYear[lang]; }
QString Translate::enterMonth() {return _enterMonth[lang]; }
QString Translate::january() {return _january[lang]; }
QString Translate::february() {return _february[lang]; }
QString Translate::march() {return _march[lang]; }
QString Translate::april() {return _april[lang]; }
QString Translate::may() {return _may[lang]; }
QString Translate::june() {return _june[lang]; }
QString Translate::july() {return _july[lang]; }
QString Translate::august() {return _august[lang]; }
QString Translate::september() {return _september[lang]; }
QString Translate::october() {return _october[lang]; }
QString Translate::november() {return _november[lang]; }
QString Translate::december() {return _december[lang]; }
QString Translate::pdfDocuments() {return _pdfDocuments[lang] ;}
QString Translate::saveFile() {return _saveFile[lang]; }
QString Translate::repeatDays() {return _repeatDays[lang]; }
QString Translate::plant() {return _plant[lang]; }
QString Translate::activitiesScheduledForThisDate() {return _activitiesScheduledForThisDate[lang]; }
QString Translate::newWorker() {return _newWorker[lang]; };
QString Translate::pleaseWait() {return _pleaseWait[lang]; };
QString Translate::yourFileHasChanged() {return _yourFileHasChanged[lang]; };
QString Translate::clearCache() {return _clearCache[lang]; };
QString Translate::youMustLoadActivities() {return _youMustLoadActivities[lang]; };
QString Translate::load() {return _load[lang]; };
QString Translate::noActivities() {return _noActivities[lang]; };
QString Translate::creditCardNumber() {return _creditCardNumber[lang]; };
QString Translate::expirationDate() {return _expirationDate[lang]; };
QString Translate::securityCode() {return _securityCode[lang]; };
QString Translate::invalidSerialKey() {return _invalidSerialKey[lang]; };
QString Translate::purchaseGardenPlanner() {return _purchaseGardenPlanner[lang]; };

QString Translate::addActivity(){return _addActivity[lang]; }
QString Translate::description(){return _description[lang]; }
QString Translate::date(){return _date[lang]; }
QString Translate::durationDays(){return _durationDays[lang]; }
QString Translate::repeatEveryXDays(){return _repeatEveryXDays[lang]; }
QString Translate::addWorkerToActivity(){return _addWorkerToActivity[lang]; }
QString Translate::select(){return _select[lang]; }
QString Translate::name(){return _name[lang]; }
QString Translate::add(){return _add[lang]; }
QString Translate::assignedWorkers(){return _assignedWorkers[lang]; }
QString Translate::remove(){return _remove[lang]; }
QString Translate::cancel(){return _cancel[lang]; }
QString Translate::createGarden(){return _createGarden[lang]; }
QString Translate::gardenName(){return _gardenName[lang]; }
QString Translate::save(){return _save[lang]; }
QString Translate::customPlants(){return _customPlants[lang]; }
QString Translate::region(){return _region[lang]; }
QString Translate::login(){return _login[lang]; }
QString Translate::username(){return _username[lang]; }
QString Translate::password(){return _password[lang]; }
QString Translate::home(){return _home[lang]; }
QString Translate::goHome(){return _goHome[lang]; }
QString Translate::settings(){return _settings[lang]; }
QString Translate::register_(){return _register_[lang]; }
QString Translate::logout(){return _logout[lang]; }
QString Translate::addGarden(){return _addGarden[lang]; }
QString Translate::viewCalendar(){return _viewCalendar[lang]; }
QString Translate::viewGardens(){return _viewGardens[lang]; }
QString Translate::gardens(){return _gardens[lang]; }
QString Translate::confirmPassword(){return _confirmPassword[lang]; }
QString Translate::theme(){return _theme[lang]; }
QString Translate::white(){return _white[lang]; }
QString Translate::dark(){return _dark[lang]; }
QString Translate::fontSize(){return _fontSize[lang]; }
QString Translate::language(){return _language[lang]; }
QString Translate::defaultSettings(){return _defaultSettings[lang]; }
QString Translate::apply(){return _apply[lang]; }
QString Translate::viewing(){return _viewing[lang]; }
QString Translate::editGarden(){return _editGarden[lang]; }
QString Translate::deleteGarden(){return _deleteGarden[lang]; }
QString Translate::activities(){return _activities[lang]; }
QString Translate::viewSlashEdit(){return _viewSlashEdit[lang]; }
QString Translate::delete_(){return _delete_[lang]; }
QString Translate::makeMonthlyReport(){return _makeMonthlyReport[lang]; }
QString Translate::welcome(){return _welcome[lang]; }
QString Translate::youAreNotLoggedIn(){return _youAreNotLoggedIn[lang]; }
QString Translate::user(){return _user[lang]; }
QString Translate::clear(){return _clear[lang]; }

std::string Translate::plant1() {return _plant1[lang]; }
std::string Translate::plant2() {return _plant2[lang]; }
std::string Translate::plant3() {return _plant3[lang]; }
std::string Translate::plant4() {return _plant4[lang]; }
std::string Translate::plant5() {return _plant5[lang]; }
std::string Translate::plant6() {return _plant6[lang]; }
std::string Translate::plant7() {return _plant7[lang]; }
std::string Translate::plant8() {return _plant8[lang]; }
std::string Translate::plant9() {return _plant9[lang]; }
std::string Translate::plant10() {return _plant10[lang]; }
std::string Translate::plant11() {return _plant11[lang]; }
std::string Translate::plant12() {return _plant12[lang]; }
std::string Translate::plant13() {return _plant13[lang]; }
std::string Translate::plant14() {return _plant14[lang]; }
std::string Translate::plant15() {return _plant15[lang]; }
std::string Translate::plant16() {return _plant16[lang]; }
std::string Translate::plant17() {return _plant17[lang]; }
std::string Translate::plant18() {return _plant18[lang]; }
std::string Translate::plant19() {return _plant19[lang]; }
std::string Translate::plant20() {return _plant20[lang]; }
std::string Translate::plant21() {return _plant21[lang]; }
std::string Translate::plant22() {return _plant22[lang]; }
std::string Translate::plant23() {return _plant23[lang]; }
std::string Translate::plant24() {return _plant24[lang]; }
std::string Translate::plant25() {return _plant25[lang]; }
std::string Translate::plant26() {return _plant26[lang]; }
std::string Translate::plant27() {return _plant27[lang]; }
std::string Translate::plant28() {return _plant28[lang]; }
std::string Translate::plant29() {return _plant29[lang]; }
std::string Translate::plant30() {return _plant30[lang]; }
std::string Translate::plant31() {return _plant31[lang]; }
std::string Translate::plant32() {return _plant32[lang]; }
std::string Translate::plant33() {return _plant33[lang]; }
std::string Translate::plant34() {return _plant34[lang]; }
std::string Translate::plant35() {return _plant35[lang]; }
std::string Translate::plant36() {return _plant36[lang]; }
std::string Translate::plant37() {return _plant37[lang]; }
std::string Translate::plant38() {return _plant38[lang]; }
std::string Translate::plant39() {return _plant39[lang]; }
std::string Translate::plant40() {return _plant40[lang]; }
std::string Translate::plant41() {return _plant41[lang]; }
std::string Translate::plant42() {return _plant42[lang]; }
std::string Translate::plant43() {return _plant43[lang]; }
std::string Translate::plant44() {return _plant44[lang]; }
std::string Translate::plant45() {return _plant45[lang]; }
std::string Translate::plant46() {return _plant46[lang]; }
std::string Translate::plant47() {return _plant47[lang]; }
std::string Translate::plant48() {return _plant48[lang]; }
std::string Translate::plant49() {return _plant49[lang]; }
std::string Translate::plant50() {return _plant50[lang]; }
std::string Translate::plant51() {return _plant51[lang]; }
std::string Translate::plant52() {return _plant52[lang]; }
std::string Translate::plant53() {return _plant53[lang]; }
std::string Translate::plant54() {return _plant54[lang]; }
std::string Translate::plant55() {return _plant55[lang]; }
std::string Translate::plant56() {return _plant56[lang]; }
std::string Translate::plant57() {return _plant57[lang]; }




