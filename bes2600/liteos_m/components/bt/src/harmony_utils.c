#include "ohos_bt_utils.h"

int BleGattsUuidCharToHex(char* uuid_string, uint8_t *uuid)
{
    int n = 0;

    for (int i = 0; uuid_string[i]; i += ((uuid_string[i+2] == '-')? 3:2))
    {
        if (uuid_string[i] >= 'A' && uuid_string[i] <= 'F')
        {
            uuid[n] = uuid_string[i] - 'A' + 10;
        }
        else
        {
            uuid[n] = uuid_string[i] - '0';
        }

        if (uuid_string[i+1] >= 'A' && uuid_string[i+1] <= 'F')
        {
            uuid[n] = (uuid[n] << 4) | (uuid_string[i+1] - 'A' + 10);
        }
        else
        {
            uuid[n] = (uuid[n] << 4) | (uuid_string[i+1] - '0');
        }
        ++n;
    }

    return (n > 2)? 16:2;
}

int BleGattsUuidHexToChar(const uint8_t *uuid, uint8_t uuid_len, char *uuid_string)
{
    int n = 0;
    uint8_t temp_data = 0;

    for (int i=0; i < uuid_len; ++i)
    {
        if ((n == 8) || (n == 13) || (n == 18) || (n ==23))
        {
            uuid_string[n] = '-';
            n++;
        }

        temp_data = uuid[i] >> 4;
        if (temp_data<10)
        {
            uuid_string[n] = temp_data + '0';
        }
        else
        {
            uuid_string[n] = (temp_data-10) + 'A';
        }
        n++;

        temp_data = uuid[i] & 0x0F;
        if (temp_data<10)
        {
            uuid_string[n] = temp_data + '0';
        }
        else
        {
            uuid_string[n] = (temp_data-10) + 'A';
        }
        n++;
    }

    return n;
}

