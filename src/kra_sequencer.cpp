#include "kra_sequencer.h"

// *********** promene pro spravu instanci KRA_Sequencer ****************
int KRA_Sequencer::numberInstanc = 0;
KRA_Sequencer *KRA_Sequencer::pointerInstanc[MAX_NUMBER_INSTANC] = {};

KRA_Sequencer::KRA_Sequencer() : functionSequenc{},
                                 typeFunction{},
                                 commandItem{},
                                 sequencActual(""),
                                 sequencBuffer{},
                                 isRunIndikator(3)
{
  for (int i = 0; i < MAX_FUNCTION_BUFFER; i++)
  {
    typeFunction[i] = "";
  }
  for (int i = 0; i < MAX_SEQUNC_BUFFER; i++)
  {
    sequencBuffer[i] = "";
  }
  // sprava instanci
  // nalezení prázdného záznamu v tabulce instalovaných vrstev
  for (int i = 0; i < MAX_NUMBER_INSTANC; i++)
  {
    if (pointerInstanc[i] == 0)
    {
      // uložení prázdné pozice a její obsazení nově vytvořenou vrstvou
      pointerInstanc[i] = this;
      numberInstanc++; // zvýšení počtu instalovaných vrstev
      break;
    }
  }
}
KRA_Sequencer::~KRA_Sequencer()
{

  for (int i = 0; i < MAX_NUMBER_INSTANC; i++)
  { // vyhledání rušené vrstvy
    if (pointerInstanc[i] == this)
    {
      pointerInstanc[i] = 0; //deaktivace rušené vrstvy.
    }
    numberInstanc--;
    break;
  }
}

void KRA_Sequencer::execute()
{

  for (int i = 0; i < MAX_NUMBER_INSTANC; i++)
  {
    if (!pointerInstanc[i] == 0)
    {
      pointerInstanc[i]->executeItem();
    }
  }
}
bool KRA_Sequencer::executeItem()
{
  for (int i = 0; i < MAX_ASINCHRON_COMMAND; i++)
  {
    if (i == 0 || commandItem[i].used)
    {
      //      commandItem[i].printDebug();
      if (commandItem[i].used)
      {
        if (commandItem[i].commandTimes == 0)
        { // start casovani
          while ((commandItem[i].commandTimes = millis()) == 0)
          {
          };
        }
        /*
        // volani pri startu
        if (commandItem[i].callCommandStart && commandItem[i].commandTimes == 0) {
          //    Serial.println("callCommandStart");
          callCommandFunction(&commandItem[i]);
        }
        // volani po skoncenicasu
        else if (commandItem[i].callCommandStop && commandItem[i].commandTimes == -1) {
          callCommandFunction(&commandItem[i]);
        }
        */
        // cyklicke volani v prubehu

        //        if ((millis() - (unsigned long)commandItem[i].commandTimes) >= commandItem[i].timeWaitingCommand &&
        //                 (commandItem[i].timeActionCommand == 0 ||
        //                  (millis() - (unsigned long)commandItem[i].commandTimes <= commandItem[i].timeActionCommand)))
        if ((millis() - (unsigned long)commandItem[i].commandTimes) >= commandItem[i].timeWaitingCommand)
        {
          callCommandFunction(&commandItem[i]);
          if (commandItem[i].assinchronAction)
            i = moveToAssinchron();
        }
      }
      // casovani command je 0, nebo 1, to znamaná pouze jednorázové spuštění
      if (commandItem[i].timeActionCommand <= 1)
      {
        commandItem[i].anNext = true;
      }
      else if ((millis() - ((unsigned long)commandItem[i].commandTimes)) >= commandItem[i].timeActionCommand)
      { // cas command vyprsel
        commandItem[i].anNext = true;
      }
    }
    // posunuti na dalsi command
    if (commandItem[i].anNext)
    {
      commandItem[i].used = false;
      if (i == 0)
      {
        commandItem[0].anBreak = commandItem[i].anBreak || !nextCommand(); // nacte dalsi command. Pokud zadny neni nastavi prikaz anBreak pro nacteni dalsi sequnce
      }
      else
      {
        commandItem[i].anBreak = true;
      }
    }
    // posunuti na dalsi sequenc a jeji prvni command
    if (i == 0 && commandItem[i].anBreak)
    {
      getNextSequence();
      nextCommand();
    }
  } // for MAX_ASINCHRON_COMMAND

  /*
    // volani pri startu
    if (callCommandStart && commandTimes == 0) {
    callCommandFunction(commandIndex);
    }
    // volani po skoncenicasu
    else if (callCommandStop && commandTimes == -1) {
    callCommandFunction(commandIndex);
    }
    // cyklicke volani v prubehu
    else if (callCommandInTime && (millis() - ((unsigned long)commandTimes)) < timeActionCommand) {
    callCommandFunction(commandIndex);
    }

    // casovani command
    if (commandTimes == 0) { // start casovani
    commandTimes = millis();
    if (commandTimes == 0)commandTimes = 1;
    } else if (timeActionCommand == 0) {
    anNext = true;
    } else if (commandTimes == -1) {
    anNext = true;
    } else if ((millis() - ((unsigned long)commandTimes)) >= timeActionCommand) { // cas command vyprsel
    commandTimes = -1;
    }

    // posunuti na dalsi command
    if (anNext) {
    anBreak = anBreak || !nextCommand(); // nacte dalsi command. Pokud zadny neni nastavi prikaz anBreak pro nacteni dalsi sequnce
    }
    // posunuti na dalsi sequenc a jeji prvni command
    if (anBreak) {
    getNextSequence();
    nextCommand();
    }

  */
  //  if(command.equals(""))nextCommand();
  return true;
}

bool KRA_Sequencer::addCommand(String command)
{
  int i;
  if ((i = findFreeCommand()) > 0)
  {
    commandItem[i] = getCommand(&command);
    return true;
  }
  return false;
}

int KRA_Sequencer::findFreeCommand()
{
  for (int i = 1; i < MAX_ASINCHRON_COMMAND; i++)
  {
    if (!commandItem[i].used)
    {
      return i;
    }
  }
  return -1;
}

int KRA_Sequencer::moveToAssinchron()
{
  int i;
  if ((i = findFreeCommand()) >= 0)
  {
    commandItem[i] = commandItem[0];
    commandItem[0].used = false;
    commandItem[0].assinchron = false;
    commandItem[0].command = "";
    commandItem[0].timeActionCommand = 0;
  }
  return i;
}

bool KRA_Sequencer::nextCommand()
{
  //  while (getNextCommand()) {}
  //  getNextCommand();
  commandItem[0] = getCommand(&sequencActual);
  if (!commandItem[0].used)
  {
    commandItem[0].anNext = true;
    return false;
  }
  else
  {
    //    commandItem[0].printDebug();
    if ((commandItem[0].commandIndex = findCommandIndex(commandItem[0].command)) == -1)
      commandItem[0].used = false;
    if (commandItem[0].assinchronWaiting)
    {
      moveToAssinchron();
    }
  }
  //  if (commandItem[0].command.equals(""))return false;
  //  debPrintCommand();
  return true;
}
int KRA_Sequencer::findCommandIndex(String command)
{
  if (!command.equals(""))
  {
    //    Serial.println("#sequencer DEBUG: hledam:" + command);
    for (int i = 0; i < MAX_FUNCTION_BUFFER; i++)
    {
      //    Serial.println("#sequencer DEBUG: "+String(i)+":" + typeFunction[i]);
      if (typeFunction[i].equals(command))
      {
        //    Serial.println("#sequencer DEBUG: nenalezeny prikaz:" + command);
        return i;
      }
    }
    Serial.println("#sequencer ERROR: nenalezeny prikaz:" + command);
  }
  return -1;
}
/**
 * 
 * @param *String
 * @return KRA_Command
 */
KRA_Command KRA_Sequencer::getCommand(String *commandIn)
{
  KRA_Command com;
  unsigned int indexRead = 0;
  int countParametr = 0;
  //  String *commandIn=commandInput;
  com.pointerSequencer = this;
  com.command = "";
  com.commandValue = "";
  com.assinchron = false;
  com.used = false;
  com.assinchronAction = false;
  com.assinchronWaiting = false;
  com.commandTimes = 0;
  com.timeActionCommand = 0;
  com.timeWaitingCommand = 0;
  com.anNext = false;
  com.anBreak = false;
  String timeActionString = "";
  String timeWaitingString = "";
  if (commandIn->length() > 0)
  {
    while (indexRead < commandIn->length())
    {
      //          Serial.print("("+String(indexRead)+")");
      //          Serial.print(commandIn->charAt(indexRead));
      if (countParametr == 0 && commandIn->charAt(indexRead) == ',')
      { // ještě nebyla závorka
        indexRead++;
        break;
      }
      else if (countParametr > 7 && commandIn->charAt(indexRead) == ',')
      { //pokud je po konci závorky čárka
        indexRead++;
        break;
      }
      else if (countParametr > 5 && commandIn->charAt(indexRead) != ' ')
      { //pokud není po konci závorky mezera
        break;
      }
      else if (countParametr == 0 && commandIn->charAt(indexRead) == '(')
      {
        countParametr++;
      }
      else if (countParametr == 3 && commandIn->charAt(indexRead) == '"')
      {
        countParametr++;
      }
      else if (countParametr == 4 && commandIn->charAt(indexRead) == '"')
      {
        countParametr++;
      }
      else if (countParametr == 4 && commandIn->charAt(indexRead) == '\\')
      {
        if (commandIn->charAt(indexRead + 1) == '\\' || commandIn->charAt(indexRead + 1) == '"')
        {
          com.commandValue += commandIn->charAt(indexRead + 1);
          indexRead++;
        }
      }
      else if (countParametr > 0 && countParametr < 3 && commandIn->charAt(indexRead) == ',')
      {
        countParametr++;
        //        if (countParametr > 5) countParametr = 5;
      }
      else if (countParametr > 0 && countParametr != 4 && commandIn->charAt(indexRead) == ')')
      {
        countParametr = 8;
      }
      else if (countParametr == 0)
      {
        com.command += commandIn->charAt(indexRead);
      }
      else if (countParametr == 1)
      {
        if (commandIn->charAt(indexRead) >= '0' && commandIn->charAt(indexRead) <= '9')
          timeWaitingString += commandIn->charAt(indexRead);
        ;
        if (commandIn->charAt(indexRead) == 'A' || commandIn->charAt(indexRead) == 'a')
          com.assinchronWaiting = true;
      }
      else if (countParametr == 2)
      {
        if (commandIn->charAt(indexRead) >= '0' && commandIn->charAt(indexRead) <= '9')
          timeActionString += commandIn->charAt(indexRead);
        ;
        if (commandIn->charAt(indexRead) == 'A' || commandIn->charAt(indexRead) == 'a')
          com.assinchronAction = true;
      }
      else if (countParametr == 3 || countParametr == 4)
      {
        com.commandValue += commandIn->charAt(indexRead);
      }
      indexRead++;
    }
    commandIn->remove(0, indexRead); // odřízne načtená data
  }
  timeWaitingString.trim();
  com.timeWaitingCommand = timeWaitingString.toInt();
  timeActionString.trim();
  com.timeActionCommand = com.timeWaitingCommand + timeActionString.toInt();
  com.command.trim();
  //   if (!com.command.equals(""))com.printDebug();
  if (!com.command.equals(""))
  {
    com.used = true;
    isRunIndikator = 3;
    return com;
  }
  isRunIndikator = (isRunIndikator > 0) ? isRunIndikator - 1 : isRunIndikator;
  return com;
}

bool KRA_Sequencer::getNextSequence()
{
  sequencActual = sequencBuffer[0];
  if (!sequencActual.equals(""))
  {
    for (int i = 0; i < MAX_SEQUNC_BUFFER - 1; i++)
    {
      sequencBuffer[i] = sequencBuffer[i + 1];
    }
    sequencBuffer[MAX_SEQUNC_BUFFER - 1] = "";
    return true;
  }
  return false;
}
bool KRA_Sequencer::addSequenc(String sequenc)
{
  for (int i = 0; i < MAX_SEQUNC_BUFFER - 1; i++)
  {
    if (sequencBuffer[i].equals(""))
    {
      sequencBuffer[i] = String(sequenc);
      isRunIndikator = 3;
      return true;
    }
  }
  return false;
}
bool KRA_Sequencer::callCommandFunction(KRA_Command *command)
{
  //  int commandIndex=command->commandIndex;
  //  Serial.println("bool KRA_Sequencer::callCommandFunction(" + String(command->commandIndex) + ") used:" + String(command->used ? "true" : "false") + " command:" + command->command + "(" + (command->callCommandStart ? "1" : "0") + "," + (command->callCommandInTime ? "1" : "0") + "," + (command->callCommandStop ? "1" : "0") + "," + String(command->timeActionCommand) + ") times=" + String( millis() - ((unsigned long)command->commandTimes)));
  if (command->commandIndex >= 0 && command->commandIndex < MAX_FUNCTION_BUFFER && functionSequenc[command->commandIndex] != 0)
  {
    //    Serial.println("#sequncer call command:"+command->command+" vith Value:"+command->commandValue);
    (functionSequenc[command->commandIndex])(command, command->commandValue);
    //    Serial.println("#sequncer finish command"+command->command);
    return true;
  }
  return false;
}
/**
 * registrace prikazu command a prislusne funkce
 * @param String - jmeno prikazu
 * @param function; void (&functionOn)(KRA_Command *pointer, String value) - registrovana funkce 
 * @return
 */
bool KRA_Sequencer::onCommand(String sequnceName, void (&functionOn)(KRA_Command *pointer, String value))
{

  for (int i = 0; i < MAX_FUNCTION_BUFFER; i++)
  {
    if ((functionSequenc[i]) == 0)
    {
      functionSequenc[i] = functionOn;
      //      delete(typeFunction[i]);
      //      typeFunction[i] = new String(sequnceName);
      typeFunction[i] = String(sequnceName);
      return true;
    }
  }
  return false;
}
/**
 * registrace prikazu command a prislusne funkce
 * @param char[] - jmeno prikazu
 * @param function; void (&functionOn)(KRA_Command *pointer, String value) - registrovana funkce 
 * @return
 */
bool KRA_Sequencer::onCommand(const char sequnceName[], void (&functionOn)(KRA_Command *pointer, String value))
{
  return onCommand(String(sequnceName), (functionOn));
}
