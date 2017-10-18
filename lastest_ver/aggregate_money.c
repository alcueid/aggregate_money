/*
  毎月の収支ファイルを集計するプログラム
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define STR_LINE_MAX 1024  // max size of subscript

int advance_ptr_to_space(char []);

char *insert_comma(int);

int read_file(char [], int []);

int append_file(char [], int []);


// advance the ptr to space and return pointer
int advance_ptr_to_space(char str[]) {
  int i = 0;
  while (str[i++] != ' ') {
  }

  return i;
}

// kannma format
char *insert_comma(int yen) {
  static char result[30];
  char tmp[30];
  int start = 0, sign = 0;
  int i1, i2;

  // absorb sign
  if (yen < 0) {
	sign = -1;
	yen = -yen;
  } else {
	sign = 1;
  }

  sprintf(tmp, "%d", yen);
  start = strlen(tmp) % 3;
  if (start == 0) start = 3;

  i2 = 0;
  for (i1 = 0; ; i1++) {
	result[i2++] = tmp[i1];
	if (tmp[i1] == '\0') {
	  break;
	}
	start--;
	if (start == 0 && tmp[i1+1] != '\0') {
	  result[i2++] = ',';
	  start = 3;
	}
  }
  if (sign < 0) {
	strcpy(tmp, result);
	sprintf(result, "-%s", tmp);
  }
  return (result);
}

// read file and aggregate data
int read_file(char filename[], int res[]) {
  FILE *fp;
  char str[STR_LINE_MAX];
  char skip_until[] = "# DETAIL\n";
  int i;
  char option;
  char plus_or_minus;

  // open the file
  if ((fp = fopen(filename, "r")) == NULL) {
	printf("I can not opne the %s\n", filename);
	return (-1);
  }

  // advance the ptr to # DETAIL in the file
  while (strcmp(fgets(str, STR_LINE_MAX, fp), skip_until) != 0) ;
  
  // advance the ptr to option in the string
  while (!((fgets(str, STR_LINE_MAX, fp) == NULL) || str[0] == '\n')) {
    i = advance_ptr_to_space(str);  // advance the ptr to content
    i += advance_ptr_to_space(str + i); // advance the ptr to haifun

    option = str[++i];  // advance the ptr to option char and substitute
    i += advance_ptr_to_space(str + i); // advance ptr to plus or minus
    plus_or_minus = str[i++];           // substitute plus or minus and advance ptr

    // aggregate result
    switch(plus_or_minus) {
    case '+':
      switch(option) {
      case 'w':
        res[0] += atoi(&str[i]);
        break;
	  case 'b':
		res[1] += atoi(&str[i]);
		break;
      case 'c':
        res[2] += atoi(&str[i]);
        break;
	  case 'e':
		res[3] += atoi(&str[i]);
		break;
	  case 'p':
		res[4] += atoi(&str[i]);
		break;
	  case 'g':
		res[5] += atoi(&str[i]);
		break;
      default:
        printf("read_file(): unkown option %c\n", option);
        break;
      }
      break;
    case '-':
      switch(option) {
      case 'w':
        res[6] -= atoi(&str[i]);
        break;
	  case 'b':
		res[7] -= atoi(&str[i]);
		break;
      case 'c':
        res[8] -= atoi(&str[i]);
        break;
      case 'e':
        res[9] -= atoi(&str[i]);
        break;
      case 'p':
        res[10] -= atoi(&str[i]);
        break;
      case 'g':
        res[11] -= atoi(&str[i]);
        break;
      default:
        printf("read_file(): unkown option %c\n", option);
        break;
      }
      break;
    default:
      printf("read_file(): unkown plus_or_minus %c\n", plus_or_minus);
      break;
    }

	// initialize
	option = '\0';
	plus_or_minus = '\0';
  }

  // close file
  fclose(fp);

  return 0;
}


// apend the aggregate result in the file
int append_file(char filename[], int res[]) {
  FILE *fp;

  // open file
  if ((fp = fopen(filename, "a")) == NULL) {
    printf("I can not appned the %s\n", filename);
    return (-1);
  }

  // append result
  fprintf(fp, "-------------------------------------------------\n");
  fprintf(fp, "# AGGREGATE RESULTS (income)\n");
  fprintf(fp, "  Wallet          :%10s yen\n", insert_comma(res[0]));
  fprintf(fp, "  Bank            :%10s yen\n", insert_comma(res[1]));
  fprintf(fp, "  Credit          :%10s yen\n", insert_comma(res[2]));
  fprintf(fp, "  Electronic Money:%10s yen\n", insert_comma(res[3]));
  fprintf(fp, "  Prepaid Card    :%10s yen\n", insert_comma(res[4]));
  fprintf(fp, "  Gift Card       :%10s yen\n", insert_comma(res[5]));
  fprintf(fp, "  -------------------------------\n");
  fprintf(fp, "  Total           :%10s yen\n",
  		  insert_comma(res[0] + res[1] + res[2] + res[3] + res[4] + res[5]));
  fprintf(fp, "\n");
  fprintf(fp, "# AGGREGATE RESULTS (expenditure)\n");
  fprintf(fp, "  Wallet          :%10s yen\n", insert_comma(res[6]));
  fprintf(fp, "  Bank            :%10s yen\n", insert_comma(res[7]));
  fprintf(fp, "  Credit          :%10s yen\n", insert_comma(res[8]));
  fprintf(fp, "  Electronic Money:%10s yen\n", insert_comma(res[9]));
  fprintf(fp, "  Prepaid Card    :%10s yen\n", insert_comma(res[10]));
  fprintf(fp, "  Gift Card       :%10s yen\n", insert_comma(res[11]));
  fprintf(fp, "  -------------------------------\n");
  fprintf(fp, "  Total           :%10s yen\n",
  		  insert_comma(res[6] + res[7] + res[8] + res[9] + res[10] + res[11]));
  fprintf(fp, "-------------------------------------------------\n");

  // close file
  fclose(fp);

  return 0;
}

int main(int argc, char *argv[])
{
  /* array description
    aggregate_result[0]  : income on wallet
	aggregate_result[1]  : income on bank
    aggregate_result[2]  : income on credit
	aggregate_result[3]  : income on electronic money
	aggregate_result[4]  : income on prepaid money
	aggregate_result[5]  : income on gift card
    aggregate_result[6]  : expenditure on wallet
	aggregate_result[7]  : expenditure on bank
    aggregate_result[8]  : expenditure on credit
    aggregate_result[9]  : expenditure on electronic money
    aggregate_result[10] : expenditure on prepaid money
    aggregate_result[11] : expenditure on gift card


  */
  int aggregate_result[12] = {0};

  // check argument
  if (argc != 2) {
    printf("usage: %s filename\n", argv[0]);
    return (-1);
  }

  // read and aggregate
  if (read_file(argv[1], aggregate_result) == -1) {
	return (-1);
  }

  // append aggregate result
  if (append_file(argv[1], aggregate_result) == -1) {
    return (-1);
  }

  return 0;
}
