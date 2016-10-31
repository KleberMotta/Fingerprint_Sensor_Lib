// 
// 
// 

#include "fingerprintSensorLib.h"

#ifdef __AVR__
#include <util/delay.h>
#include <SoftwareSerial.h>
#endif

#ifdef __AVR__
FingerprintSensorLibClass::FingerprintSensorLibClass(SoftwareSerial *soft) {

	hardSerial = NULL;
	softSerial = soft;
	actSerial = softSerial;
}
#endif

FingerprintSensorLibClass::FingerprintSensorLibClass(HardwareSerial *hard)
{

	#ifdef __AVR__
		softSerial = NULL;
	#endif
		hardSerial = hard;
		actSerial = hardSerial;
}

bool FingerprintSensorLibClass::init(uint32_t baudRate)
{
	delay(1000);  // Espera o sensor ligar

	if (hardSerial) hardSerial->begin(baudRate);
#ifdef __AVR__
	if (softSerial) softSerial->begin(baudRate);
#endif
}

bool FingerprintSensorLibClass::start(void)
{
	FINGERPRINT_START();

	uint8_t pacoteResp[12];
	memset(pacoteResp, 0xff, 12);  //zero out template buffer
	int index = 0;
	delay(1000);
	while (index < 12)
	{
		if (actSerial->available())
		{
			pacoteResp[index] = actSerial->read();
			index++;
		}
	}

#ifdef  debug

	Serial.println("RESPOSTA HandShake");

	for (int count = 0; count < 12; count++)
	{
		Serial.print("0x");
		Serial.print(pacoteResp[count], HEX);
		Serial.print(", ");
	}

	Serial.println();
	Serial.println();

#endif

	if (pacoteResp[9] != 0x00)
	{
		//Serial.println("Sensor nao encontrado!");
		return false;
	}
	else
	{
		//Serial.println("Sensor pronto.");
		return true;
	}
}

bool FingerprintSensorLibClass::pegarImagem(void)
{
	FINGERPRINT_GET_IMAGE();

	Serial.print(".");

	uint8_t pacoteResp[12];
	memset(pacoteResp, 0xff, 12);  //zero out template buffer
	int index = 0;
	delay(1000);

	while (index < 12)
	{
		if (actSerial->available())
		{
			pacoteResp[index] = actSerial->read();
			index++;
		}
	}

#ifdef debug

	Serial.println("RESPOSTA getImage");

	for (int count = 0; count < 12; count++)
	{
		Serial.print("0x");
		Serial.print(pacoteResp[count], HEX);
		Serial.print(", ");
	}

	Serial.println();
	Serial.println();

#endif

	if (pacoteResp[9] != 0x00)
	{
		//Serial.println("Erro ao pegar imagem");
		return false;
	}
	else
	{
		//Serial.println("\nImagem armazenada");
		return true;
	}
}

bool FingerprintSensorLibClass::img2tz(byte param)
{
	FINGERPRINT_2_TZ(param);

	uint8_t pacoteResp[12];
	memset(pacoteResp, 0xff, 12);  //zero out template buffer
	int index = 0;
	delay(1000);
	while (index < 12)
	{
		if (actSerial->available())
		{
			pacoteResp[index] = actSerial->read();
			index++;
		}
	}

#ifdef  debug

	Serial.println("RESPOSTA img2tz");

	for (int count = 0; count < 12; count++)
	{
		Serial.print("0x");
		Serial.print(pacoteResp[count], HEX);
		Serial.print(", ");
	}

	Serial.println();
	Serial.println();

#endif

	if (pacoteResp[9] != 0x00)
	{
		Serial.println("Problema na Conversão!");
		return false;
	}
	else
	{
		//Serial.println("Imagem Convertida para Char.");
		return true;
	}
}

bool FingerprintSensorLibClass::criarModelo(void)
{
	FINGERPRINT_GEN_TEMPLATE();

	uint8_t pacoteResp[12];
	memset(pacoteResp, 0xff, 12);  //zero out template buffer
	int index = 0;
	delay(1000);

	while (index < 12)
	{
		if (actSerial->available())
		{
			pacoteResp[index] = actSerial->read();
			index++;
		}
	}

#ifdef debug

	Serial.println("RESPOSTA GenModel");

	for (int count = 0; count < 12; count++)
	{
		Serial.print("0x");
		Serial.print(pacoteResp[count], HEX);
		Serial.print(", ");
	}

	Serial.println();
	Serial.println();

#endif

	if (pacoteResp[9] != 0x00)
	{
		//Serial.println("Erro ao pegar imagem");
		return false;
	}
	else
	{
		//Serial.println("\nTemplate Gerada");
		return true;
	}
}

bool FingerprintSensorLibClass::excluirDB(void)
{
	FINGERPRINT_DELETE_ALL();

	uint8_t pacoteResp[12];
	memset(pacoteResp, 0xff, 12);  //zero out template buffer
	int index = 0;
	delay(1000);

	while (index < 12)
	{
		if (actSerial->available())
		{
			pacoteResp[index] = actSerial->read();
			index++;
		}
	}

#ifdef debug

	Serial.println("RESPOSTA Empty");

	for (int count = 0; count < 12; count++)
	{
		Serial.print("0x");
		Serial.print(pacoteResp[count], HEX);
		Serial.print(", ");
	}

	Serial.println();
	Serial.println();

#endif

	if (pacoteResp[9] != 0x00)
	{
		//Serial.println("Erro ao pegar imagem");
		return false;
	}
	else
	{
		//Serial.println("\nTemplates apagadas");
		return true;
	}
}


bool FingerprintSensorLibClass::pegarModelo(char * buff)
{
	FINGERPRINT_GET_TEMPLATE();

	memset(buff, 0xff, 512); //zera todo mundo

	uint8_t pacoteResp[12];
	memset(pacoteResp, 0xff, 12);  //zero out template buffer
	int index = 0;
	delay(1000);
	while (index < 12) // Esse laço armazena o pacote de confirmação
	{
		if (actSerial->available())
		{
			pacoteResp[index] = actSerial->read();
			index++;
		}
	}

#ifdef  debug

	Serial.println("RESPOSTA UpChar");

	for (int count = 0; count < 12; count++)
	{
		Serial.print("0x");
		Serial.print(pacoteResp[count], HEX);
		Serial.print(", ");
	}

	Serial.println();
	Serial.println();

#endif

	if (pacoteResp[9] != 0x00) // Significa que o leitor não foi capaz de executar a intrução UpChar
	{
		Serial.println("\n\nErro no UpChar!");
		return false;
	}

	// começando código que vai iterar o buffer!
	uint8_t count = 0;
	uint8_t num = 1;

	while (true)
	{
		for (uint8_t i = 0; i < 12; i++) // Esse laço vai armazenar a confirmação de envio de dados, que para cada pacote em cada pacote de 136
										 // Sendo exatamente os 11 primeiros bytes de transimissão, enqaunto os 128 restantes são armazenados no buffer
		{
			if (actSerial->available())
			{
				pacoteResp[index] = actSerial->read();
				index++;
			}
		}

		while (count < 128 * num) // esse laço vai iterando o buffer, o 128 é o numero de bytes por pacote por padrão, podendo ser alterado no 'sysParamters'
		{
			if (actSerial->available())
			{
				buff[count] = actSerial->read(); // vai armazenando os valores no buffer
			}
		}

		if (pacoteResp[6] == 0x08) // quando recebermos a confirmação de "final do pacote" saímos do laço :)
		{
			break;
		}

		num *= 2; // Isso vai continuar a iteração do buff :P MuuuahAHahHAhHhAhAhA
	}

	//Serial.println("Template extraída com sucesso");
	return true;
}

 /*
	Funções privadas, responsáveis por enviarem as intruções para o módulo
 */

void FingerprintSensorLibClass::FINGERPRINT_START(void)
{
	byte FP_VFY_PWD[] = { 0xEF, 0x1, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x0, 0x7, 0x13, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1B };
	actSerial->write(FP_VFY_PWD, sizeof(FP_VFY_PWD));
}

void FingerprintSensorLibClass::FINGERPRINT_GET_IMAGE()
{
	byte FP_GET_IMAGE[] = { 0xef, 0x01, 0xff, 0xff, 0xff, 0xff, 0x1, 0x0, 0x3, 0x01, 0x00, 0x5 };
	actSerial->write(FP_GET_IMAGE, sizeof(FP_GET_IMAGE));
}

void FingerprintSensorLibClass::FINGERPRINT_2_TZ(byte param)
{
	byte sum = 0x1 + 0x0 + 0x4 + 0x2 + param; // Como o parâmetro pode variar...

	byte FP_IMG_2_TZ[] = { 0xef, 0x01, 0xff, 0xff, 0xff, 0xff, 0x1, 0x0, 0x4, 0x2, param, 0x0, sum };
	actSerial->write(FP_IMG_2_TZ, sizeof(FP_IMG_2_TZ));
}

void FingerprintSensorLibClass::FINGERPRINT_GEN_TEMPLATE(void)
{
	byte FP_GEN_TEMPLATE[] = { 0xEF, 0x1, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x0, 0x3, 0x5, 0x0, 0x9 };
	actSerial->write(FP_GEN_TEMPLATE, sizeof(FP_GEN_TEMPLATE));
}

void FingerprintSensorLibClass::FINGERPRINT_GET_TEMPLATE(void)
{
	byte FP_GET_TEMPLATE[] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x0, 0x4, 0x8, 0x1, 0x0, 0xE5 };
	actSerial->write(FP_GET_TEMPLATE, sizeof(FP_GET_TEMPLATE));
}

void FingerprintSensorLibClass::FINGERPRINT_DELETE_ALL(void)
{
	byte FP_EMPTY[] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x0, 0x3, 0xd, 0x0, 0x11 };
	//EF01H Xxxx 01H 0003H 0dH 0011H 
	actSerial->write(FP_EMPTY, sizeof(FP_EMPTY));
}


