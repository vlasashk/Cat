#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ONE_PAGE 4096
#define ARGV_ARR 64

typedef struct {
  int number_nonempty;   // -b
  int swap_end_of_line;  // -E
  int number_output;     // -n
  int squeeze_blank;     // -s
  int show_tabs;         // -T
  int non_printing_char; // -v

  int array_counter;
  unsigned files_index[ARGV_ARR];
} CatOptions;

typedef struct {
  int line_number;
  int empty_line_number;
  char previous_symbol;
  unsigned char next_symbol;
  char pre_previous;
} VarsForCat;

void EmptyArgs();
void InitializeStruct(CatOptions *init_opt);
int OptionParser(CatOptions *options, char *argv);
int ArgsChecker(CatOptions *options, int argc, char *argv[]);
int SingleCharParser(CatOptions *options, char *argv);
void CommandProcessing(CatOptions *options, char *argv[],
                       VarsForCat *variables);
void ReadFileNoArgs(char *file_name);
void OptionsProcessing(CatOptions *options, char *file_name,
                       VarsForCat *variables);
void NumberOutput(VarsForCat *variables);
void NumberEmptyOutput(VarsForCat *variables, char symbol);
int SqueezeBlank(VarsForCat *variables, unsigned char current);
void ShowTabs(unsigned char *symbol, char version);
void CharacterProcessing(unsigned char *symbol);
void SwapEndOfline(unsigned char *symbol, char version);
void NonPrintingProcessing(CatOptions *options, unsigned char *buffer_symbol);

#endif // SRC_CAT_S21_CAT_H_
