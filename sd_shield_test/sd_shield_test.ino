#include <SD.h>
#include <SPI.h>
#include <DS1302.h>
#include <Wire.h>
#include<String.h>

DS1302 rtc(2, 3, 4);

int CS_PIN = 10;
File file;
String main_clock;


void setup()
{
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);
  // The following lines can be commented out to use the values already stored in the DS1302
  //rtc.setDOW(WEDNESDAY);        // Set Day-of-Week to FRIDAY
  //rtc.setTime(16, 7, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(27, 1, 2016);   // Set the date to August 6th, 2010

  Serial.begin(9600);
  initializeSD();
  createFile("timelog.txt");

  writeToFile("This is a time log!");

  int i;

  for (i = 0; i < 10; i++)
  {
    writeToFile(rtc.getTimeStr());
    main_clock=String(millis());
    writeToFile(main_clock);
    delay(1000);
  }

  closeFile();
}

void loop()
{
  // Display time
  //lcd.print(rtc.getTimeStr());

  // Display abbreviated Day-of-Week in the lower left corner
  //lcd.print(rtc.getDOWStr(FORMAT_SHORT));

  // Display date in the lower right corner
  //lcd.print(rtc.getDateStr());

  // Wait one second before repeating :)
  //delay (1000);
}

void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);
  if (file)
  {
    Serial.println("File created successfully.");
    return 1;
  } else
  {
    Serial.println("Error while creating file.");
    return 0;
  }
}

int writeToFile(String text)
{
  if (file)
  {
    file.println(text);
    Serial.println("Writing to file: ");
    Serial.println(text);
    Serial.println(millis());
    return 1;
  } else
  {
    Serial.println("Couldn't write to file");
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
    Serial.println("File closed");
  }
}

int openFile(char filename[])
{
  file = SD.open(filename);
  if (file)
  {
    Serial.println("File opened with success!");
    return 1;
  } else
  {
    Serial.println("Error opening file...");
    return 0;
  }
}

String readLine()
{
  String received = "";
  char ch;
  while (file.available())
  {
    ch = file.read();
    if (ch == '\n')
    {
      return String(received);
    }
    else
    {
      received += ch;
    }
  }
  return "";
}
