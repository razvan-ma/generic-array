#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <inttypes.h>
void print(void *arr, int len);
int add_last(void **arr, int *len, data_structure *data)
{
	// aici len este numarul de bytes din vector
	if (*arr == NULL) // vectorul nu are elemente inca
	{
		*arr = malloc(data->header->len + sizeof(int) + sizeof(char));
		if (*len != 0)
			return -1;
	}
	else
		*arr = realloc(*arr, data->header->len + sizeof(int) + sizeof(char) + *(len));
		// memorie pentru lungimea actuala si cea a elementului nou
	memmove(*arr + *len, &data->header->type, sizeof(char));
	memmove(*arr + *len + sizeof(char), &data->header->len, sizeof(int));
	memmove(*arr + *len + sizeof(int) + sizeof(char), data->data, data->header->len);
	//len se updateaza cu elemntul nou
	*len = *len + sizeof(char) + sizeof(int) + data->header->len;
	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	//aici len este numarul de elemente din vector
	if (index < 0)
		return -1;

	int count_before = 0, count_after = 0, i, aux = 0;
	if (index > *len)
		index = *len;
	// octetii de la inceput pana unde urmeaza sa se adauge se retin in count_before
	for (i = 0; i < index; i++)
	{
		memmove(&aux, *arr + count_before + 1, 4);
		count_before = count_before + 5 + aux;
	}
	// octetii de unde urmeaza sa introc se retin in count_before
	for (i = index; i < *len; i++)
	{
		memmove(&aux, *arr + count_before + count_after + 1, 4);
		count_after = count_after + 5 + aux;
	}
	// lungimea arrayului
	int byte_len = count_after + count_before;
	if (index >= *len)
	{
		add_last(arr, &byte_len, data);
		(*len)++;
		return 0;
	}
	// lungimea elemntului ce trebuie adaugat
	int data_len = data->header->len + sizeof(int) + sizeof(char);
	*arr = realloc(*arr, byte_len + data_len);
	// aici se muta se face spatiu pentru elementul nou
	for (i = 1; i <= count_after; i++)
		memmove(*arr + byte_len + data_len - i, *arr + byte_len - i, 1);
	// introducerea efectiva a elementului
	memmove(*arr + count_before, &data->header->type, sizeof(char));
	memmove(*arr + count_before + sizeof(char), &data->header->len, sizeof(int));
	memmove(*arr + count_before + sizeof(int) + sizeof(char), data->data, data->header->len);
	(*len)++;
	return 0;
}

void find(void *data_block, int len, int index)
{
	if (index >= len || index < 0)
		return;
	// se plimba pana la index
	int count = 0, i, aux = 0;
	for (i = 0; i < index; i++)
	{
		memmove(&aux, data_block + count + 1, 4);
		count = count + 5 + aux;
	}
	print(data_block + count, 1);
	// ma simt genial pentru linia asta nu pot sa primesc bonus
	// cateva puncte? jk
}

int delete_at(void **arr, int *len, int index)
{
	int count_before = 0, count_after = 0, i, aux = 0;
	for (i = 0; i < index; i++)
	{
		memmove(&aux, *arr + count_before + 1, 4);
		count_before = count_before + 5 + aux;
	}
	for (i = index; i < *len; i++)
	{
		memmove(&aux, *arr + count_before + count_after + 1, 4);
		count_after = count_after + 5 + aux;
	}
	int data_len;
	memmove(&data_len, *arr + count_before + 1, sizeof(int));
	data_len += 5;
	memmove(*arr + count_before, *arr + count_before + data_len, count_after - data_len);
	*arr = realloc(*arr, count_before + count_after - data_len);
	(*len)--;
	return 0;
}
void print(void *arr, int len)
{
	int i, j;
	int count = 0;// numarul  de octeti la care am ajuns
	for (i = 0; i < len; i++)
	{
		char type;
		char *s1, *s2;
		memmove(&type, arr + count, sizeof(char));
		count = count + sizeof(int) + sizeof(char);
		if (type == '1')
		{
			int8_t bancnota1, bancnota2;
			j = 0;
			// dedicator 1
			while (*((char *)(arr + count + j)) != '\0')
			{
				if (j == 0)
					s1 = malloc(1);
				else
					s1 = realloc(s1, j + 2);
				memmove(s1 + j, arr + count + j, 1);
				j++;
			}
			memmove(s1 + j, arr + count + j, 1);
			count = count + j + 1; // trecem de terminator(nu arnold tho)
			// bancnota 1
			memmove(&bancnota1, arr + count, 1);
			count++;
			// bancnota 2
			memmove(&bancnota2, arr + count, 1);
			count++;
			// dedicator 2
			j = 0;
			while (*((char *)(arr + count + j)) != '\0')
			{
				if (j == 0)
					s2 = malloc(1);
				else
					s2 = realloc(s2, j + 2);
				memmove(s2 + j, arr + count + j, 1);
				j++;
			}
			memmove(s2 + j, arr + count + j, 1);
			count = count + j + 1;
			//  the actual print
			printf("Tipul 1\n");
			printf("%s pentru %s\n", s1, s2);
			printf("%" PRId8 "\n", bancnota1);
			printf("%" PRId8 "\n", bancnota2);
		}
		if (type == '2')
		{
			int16_t bancnota1;
			int32_t bancnota2;
			j = 0;
			// dedicator 1
			while (*((char *)(arr + count + j)) != '\0')
			{
				if (j == 0)
					s1 = malloc(1);
				else
					s1 = realloc(s1, j + 2);
				memmove(s1 + j, arr + count + j, 1);
				j++;
			}
			memmove(s1 + j, arr + count + j, 1);
			count = count + j + 1; // trecem de terminator
			// bancnota 1
			memmove(&bancnota1, arr + count, 2);
			count = count + 2;
			// bancnota 2
			memmove(&bancnota2, arr + count, 4);
			count = count + 4;
			// dedicator 2
			j = 0;
			while (*((char *)(arr + count + j)) != '\0')
			{
				if (j == 0)
					s2 = malloc(1);
				else
					s2 = realloc(s2, j + 2);
				memmove(s2 + j, arr + count + j, 1);
				j++;
			}
			memmove(s2 + j, arr + count + j, 1);
			count = count + j + 1;
			//  the actual print
			printf("Tipul 2\n");
			printf("%s pentru %s\n", s1, s2);
			printf("%" PRId16 "\n", bancnota1);
			printf("%" PRId32 "\n", bancnota2);
		}
		if (type == '3')
		{
			int32_t bancnota1, bancnota2;
			j = 0;
			// dedicator 1
			while (*((char *)(arr + count + j)) != '\0')
			{
				if (j == 0)
					s1 = malloc(1);
				else
					s1 = realloc(s1, j + 2);
				memmove(s1 + j, arr + count + j, 1);
				j++;
			}
			memmove(s1 + j, arr + count + j, 1);
			count = count + j + 1; // trecem de terminator
			// bancnota 1
			memmove(&bancnota1, arr + count, 4);
			count = count + 4;
			// bancnota 2
			memmove(&bancnota2, arr + count, 4);
			count = count + 4;
			// dedicator 2
			j = 0;
			while (*((char *)(arr + count + j)) != '\0')
			{
				if (j == 0)
					s2 = malloc(1);
				else
					s2 = realloc(s2, j + 2);
				memmove(s2 + j, arr + count + j, 1);
				j++;
			}
			memmove(s2 + j, arr + count + j, 1);
			count = count + j + 1;
			//  the actual print
			printf("Tipul 3\n");
			printf("%s pentru %s\n", s1, s2);
			printf("%" PRId32 "\n", bancnota1);
			printf("%" PRId32 "\n", bancnota2);
		}
		printf("\n");
		free(s1);
		free(s2);
	}
}
int main()
{
	// the vector of bytes u have to work with
	// good luck :)
	void *arr = NULL;
	// lungimea vectorului in bytes
	int len = 0;
	// lungimea vectorului in elemente
	int count = 0;
	data_structure *data_struct = (data_structure *)malloc(sizeof(data_structure));
	data_struct->header = malloc(sizeof(head));
	char *line = malloc(256);
	while (1)
	{
		fgets(line, 256, stdin);
		int i = 0;
		while (line[i] != ' ')
			i++;
		char *cmd = malloc(i + 1);
		sscanf(line, "%s", cmd);
		//-----------------------------------------------INSERT-----------------------------------------------||
		if (!strcmp(cmd, "insert"))
		{
			int space = 0, s1_len = 0, s2_len = 0;
			i = 0; // parcurge linia
			// aflare lungime stringuri
			while (line[i] != '\n')
			{
				if (line[i] == ' ')
				{
					space++;
					i++;
				}
				else
					i++;
				if (space == 2)
					s1_len++;
				if (space == 5)
					s2_len++;
			}
			// aici cresc marimile stringurilor daca caracterul la care am ajuns se afla dupa un spatiu respectiv
			// pentru a afla lungimea stringurilor (contine si terminatorul de sir)
			char type;
			char *s1 = malloc(s1_len);
			char *s2 = malloc(s2_len);
			sscanf(line, "%s %c", cmd, &type);
			if (type == '1')
			{
				int8_t bancnota_1, bancnota_2;
				int banc1, banc2;
				sscanf(line, "%s %c %s %d %d %s", cmd, &type, s1, &banc1, &banc2, s2);
				
				data_struct->header->type = type;
				bancnota_1 = (int8_t)banc1;
				bancnota_2 = (int8_t)banc2;
				data_struct->header->len = s1_len + 1 + 1 + s2_len;

				data_struct->data = malloc(data_struct->header->len);
				memmove(data_struct->data, s1, s1_len);
				memmove(data_struct->data + s1_len, &bancnota_1, 1);
				memmove(data_struct->data + s1_len + sizeof(int8_t), &bancnota_2, 1);
				memmove(data_struct->data + s1_len + 2 * sizeof(int8_t), s2, s2_len);
				add_last(&arr, &len, data_struct);
				count++;
			}
			if (type == '2')
			{
				int16_t bancnota_1;
				int32_t bancnota_2;
				int banc1, banc2;
				sscanf(line, "%s %c %s %d %d %s", cmd, &type, s1, &banc1, &banc2, s2);
				data_struct->header->type = type;
				bancnota_1 = (int16_t)banc1;
				bancnota_2 = (int32_t)banc2;
				data_struct->header->len = s1_len + 2 + 4 + s2_len;
				data_struct->data = malloc(data_struct->header->len);
				memmove(data_struct->data, s1, s1_len);
				memmove(data_struct->data + s1_len, &bancnota_1, 2);
				memmove(data_struct->data + s1_len + sizeof(int16_t), &bancnota_2, 4);
				memmove(data_struct->data + s1_len + sizeof(int16_t) + sizeof(int32_t), s2, s2_len);w
				add_last(&arr, &len, data_struct);
				count++;
			}
			if (type == '3')
			{
				int32_t bancnota_1, bancnota_2;
				sscanf(line, "%s %c %s %d %d %s", cmd, &type, s1, &bancnota_1, &bancnota_2, s2);
				data_struct->header->type = type;
				data_struct->header->len = s1_len + 4 + 4 + s2_len;
				data_struct->data = malloc(data_struct->header->len);
				memmove(data_struct->data, s1, s1_len);
				memmove(data_struct->data + s1_len, &bancnota_1, 4);
				memmove(data_struct->data + s1_len + sizeof(int32_t), &bancnota_2, 4);
				memmove(data_struct->data + s1_len + 2 * sizeof(int32_t), s2, s2_len);
				add_last(&arr, &len, data_struct);
				count++;
			}
			free(data_struct->data);
			free(s1);
			free(s2);
		}
		//----------------------------------------------INSERT_AT-----------------------------------------------||
		if (!strcmp(cmd, "insert_at"))
		{
			int space = 0, s1_len = 0, s2_len = 0, index;
			i = 0; // parcurge linia
			// aflare lungime stringuri
			while (line[i] != '\n')
			{
				if (line[i] == ' ')
				{
					space++;
					i++;
				}
				else
					i++;
				if (space == 3)
					s1_len++;
				if (space == 6)
					s2_len++;
			}
			char type;
			char *s1 = malloc(s1_len);
			char *s2 = malloc(s2_len);
			sscanf(line, "%s %d %c", cmd, &index, &type);

			if (type == '1')
			{
				int8_t bancnota_1, bancnota_2;
				int banc1, banc2;
				sscanf(line, "%s %d %c %s %d %d %s", cmd, &index, &type, s1, &banc1, &banc2, s2);

				data_struct->header->type = type;
				bancnota_1 = (int8_t)banc1;
				bancnota_2 = (int8_t)banc2;
				data_struct->header->len = s1_len + 1 + 1 + s2_len;
				data_struct->data = malloc(data_struct->header->len);
				memmove(data_struct->data, s1, s1_len);
				memmove(data_struct->data + s1_len, &bancnota_1, 1);
				memmove(data_struct->data + s1_len + sizeof(int8_t), &bancnota_2, 1);
				memmove(data_struct->data + s1_len + 2 * sizeof(int8_t), s2, s2_len);
				add_at(&arr, &count, data_struct, index);
				count++;
			}
			if (type == '2')
			{
				int16_t bancnota_1;
				int32_t bancnota_2;
				int banc1, banc2;
				sscanf(line, "%s %d %c %s %d %d %s", cmd, &index, &type, s1, &banc1, &banc2, s2);
				data_struct->header->type = type;
				bancnota_1 = (int16_t)banc1;
				bancnota_2 = (int32_t)banc2;
				data_struct->header->len = s1_len + 2 + 4 + s2_len;
				data_struct->data = malloc(data_struct->header->len);
				memmove(data_struct->data, s1, s1_len);
				memmove(data_struct->data + s1_len, &bancnota_1, 2);
				memmove(data_struct->data + s1_len + sizeof(int16_t), &bancnota_2, 4);
				memmove(data_struct->data + s1_len + sizeof(int16_t) + sizeof(int32_t), s2, s2_len);
				add_at(&arr, &count, data_struct, index);
			}
			if (type == '3')
			{
				int32_t bancnota_1, bancnota_2;
				sscanf(line, "%s %d %c %s %d %d %s", cmd, &index, &type, s1, &bancnota_1, &bancnota_2, s2);
				data_struct->header->type = type;
				data_struct->header->len = s1_len + 4 + 4 + s2_len;
				data_struct->data = malloc(data_struct->header->len);
				memmove(data_struct->data, s1, s1_len);
				memmove(data_struct->data + s1_len, &bancnota_1, 4);
				memmove(data_struct->data + s1_len + sizeof(int32_t), &bancnota_2, 4);
				memmove(data_struct->data + s1_len + 2 * sizeof(int32_t), s2, s2_len);
				add_at(&arr, &count, data_struct, index);
				count++;
			}
			free(data_struct->data);
			free(s1);
			free(s2);
		}
		if (!strcmp(cmd, "delete_at"))
		{
			int index;
			sscanf(line, "%s %d", cmd, &index);
			delete_at(&arr, &count, index);
		}
		if (!strcmp(cmd, "find"))
		{
			int index;
			sscanf(line, "%s %d", cmd, &index);
			find(arr, count, index);
		}
		if (!strcmp(cmd, "print"))
			print(arr, count);
		if (!strcmp(cmd, "exit"))
		{
			free(cmd);
			break;
		}
		free(cmd);
	}
	free(data_struct->header);
	free(line);
	free(arr);
	free(data_struct);
	return 0;
}