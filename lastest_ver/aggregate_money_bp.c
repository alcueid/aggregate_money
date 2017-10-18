/*
  毎月の収支ファイルを集計するプログラム
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define STR_LINE_MAX 1024  // max size of subscript

int read_file(char [], int []);

int advance_ptr_to_space(char []);

int append_file(char [], int []);


// advance the ptr to space and return pointer
int advance_ptr_to_space(char str[]) {
  int i = 0;
  while (str[i++] != ' ') {
  }

  return i;
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
  while (fgets(str, STR_LINE_MAX, fp) != NULL) {
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
      case 'c':
        res[1] += atoi(&str[i]);
        break;
      default:
        printf("read_file(): unkown option %c\n", option);
        break;
      }
      break;
    case '-':
      switch(option) {
      case 'w':
        res[2] -= atoi(&str[i]);
        break;
      case 'c':
        res[3] -= atoi(&str[i]);
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
  fprintf(fp, "  Wallet:%+7d yen\n", res[0]);
  fprintf(fp, "  Credit:%+7d yen\n", res[1]);
  fprintf(fp, "# AGGREGATE RESULTS (expenditure)\n");
  fprintf(fp, "  Wallet:%+7d yen\n", res[2]);
  fprintf(fp, "  Credit:%+7d yen\n", res[3]);
  fprintf(fp, "-------------------------------------------------\n");

  // close file
  fclose(fp);

  return 0;
}

int main(int argc, char *argv[])
{
  /* array description
    aggregate_result[0]: income on wallet
    aggregate_result[1]: income on credit
    aggregate_result[2]: expenditure on wallet
    aggregate_result[3]: expenditure on credit
  */
  int aggregate_result[4] = {0};

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
