#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    EmptyArgs();
  } else {
    CatOptions cat_options;
    VarsForCat variables;
    InitializeStruct(&cat_options);
    variables.empty_line_number = 1;
    variables.line_number = 1;
    variables.next_symbol = ' ';
    variables.pre_previous = ' ';
    variables.previous_symbol = '\n';
    if (ArgsChecker(&cat_options, argc, argv) == 1) {
      CommandProcessing(&cat_options, argv, &variables);
    }
  }
  return 0;
}
int SqueezeBlank(VarsForCat *variables, unsigned char current) {
  int flag = 0;
  if ((variables->pre_previous == '\n') &&
      (variables->previous_symbol == '\n') && (current == '\n')) {
    flag = 1;
  }
  return flag;
}
void NumberEmptyOutput(VarsForCat *variables, char symbol) {
  if ((symbol != '\n') && (variables->previous_symbol == '\n')) {
    printf("%6d\t", (variables->line_number)++);
  }
}

void NumberOutput(VarsForCat *variables) {
  if (variables->previous_symbol == '\n') {
    printf("%6d\t", (variables->line_number)++);
  }
}

void CharacterProcessing(unsigned char *symbol) {
  if ((*symbol <= 31) && (*symbol != 9) && (*symbol != 10)) {
    printf("^");
    *symbol += 64;
  } else if ((*symbol >= 127) && (*symbol <= 159)) {
    if (*symbol == 127) {
      printf("^");
      *symbol -= 64;
    } else {
      printf("M-^");
      *symbol -= 64;
    }
  } else if ((*symbol >= 160) && (*symbol <= 254)) {
    printf("M-");
    *symbol -= 128;
  } else if (*symbol == 255) {
    printf("M-^");
    *symbol -= 192;
  }
}

void SwapEndOfline(unsigned char *symbol, char version) {
  if (version == 'e') {
    if (*symbol == 10) {
      printf("$");
    } else {
      CharacterProcessing(symbol);
    }
  } else {
    if (*symbol == 10) {
      printf("$");
    }
  }
}
void ShowTabs(unsigned char *symbol, char version) {
  if (version == 't') {
    if (*symbol == 9) {
      printf("^");
      *symbol += 64;
    } else {
      CharacterProcessing(symbol);
    }
  } else {
    if (*symbol == 9) {
      printf("^");
      *symbol += 64;
    }
  }
}
void OptionsProcessing(CatOptions *options, char *file_name,
                       VarsForCat *variables) {
  if (access(file_name, F_OK) != 0) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", file_name);
  } else {
    FILE *file = fopen(file_name, "r");
    unsigned char symbol;
    symbol = fgetc(file);
    while (!feof(file)) {
      unsigned char buffer_symbol;
      variables->next_symbol = fgetc(file);
      buffer_symbol = symbol;
      if (buffer_symbol == (unsigned char)EOF &&
          variables->next_symbol == (unsigned char)EOF) {
        break;
      } else {
        if (options->squeeze_blank == 1) {
          if (SqueezeBlank(variables, buffer_symbol) == 1) {
            symbol = variables->next_symbol;
            continue;
          }
        }
        if ((options->number_output == 1) && (options->number_nonempty == 0)) {
          NumberOutput(variables);
        } else if (options->number_nonempty == 1) {
          NumberEmptyOutput(variables, buffer_symbol);
        }
        if ((options->non_printing_char + options->swap_end_of_line +
             options->show_tabs) != 0) {
          NonPrintingProcessing(options, &buffer_symbol);
        }
        variables->pre_previous = variables->previous_symbol;
        variables->previous_symbol = symbol;
        symbol = variables->next_symbol;
        printf("%c", buffer_symbol);
      }
    }
    fclose(file);
  }
}

void NonPrintingProcessing(CatOptions *options, unsigned char *buffer_symbol) {
  if ((options->non_printing_char == 1) && (options->swap_end_of_line == 0) &&
      (options->show_tabs == 0)) {
    CharacterProcessing(buffer_symbol);
  }
  if (options->show_tabs == 1) {
    if (options->non_printing_char == 1) {
      ShowTabs(buffer_symbol, 't');
    } else {
      ShowTabs(buffer_symbol, 'T');
    }
  }
  if (options->swap_end_of_line == 1) {
    if (options->non_printing_char == 1) {
      SwapEndOfline(buffer_symbol, 'e');
    } else {
      SwapEndOfline(buffer_symbol, 'E');
    }
  }
}

void CommandProcessing(CatOptions *options, char *argv[],
                       VarsForCat *variables) {
  int option_checker = options->swap_end_of_line + options->number_nonempty +
                       options->number_output + options->show_tabs +
                       options->squeeze_blank + options->non_printing_char;
  if (option_checker == 0 && options->array_counter != 0) {
    for (int i = 0; i < options->array_counter; i++) {
      ReadFileNoArgs(argv[options->files_index[i]]);
    }
  } else if (option_checker != 0 && options->array_counter == 0) {
    fprintf(stderr, "s21_cat can't handle options without file\n");
  } else {
    for (int i = 0; i < options->array_counter; i++) {
      OptionsProcessing(options, argv[options->files_index[i]], variables);
    }
  }
}

void ReadFileNoArgs(char *file_name) {
  if (access(file_name, F_OK) != 0) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", file_name);
  } else {
    FILE *file = fopen(file_name, "r");
    unsigned char symbol;
    symbol = fgetc(file);
    while (!feof(file)) {
      unsigned char buffer_symbol;
      buffer_symbol = fgetc(file);
      if (symbol == (unsigned char)EOF && buffer_symbol == (unsigned char)EOF) {
        break;
      } else {
        printf("%c", symbol);
        symbol = buffer_symbol;
      }
    }
    fclose(file);
  }
}

int SingleCharParser(CatOptions *options, char *argv) {
  int flag = 1;
  for (char *c = argv; *c; c++) {
    if (flag == 1) {
      switch (*c) {
      case 'b':
        options->number_nonempty = 1;
        break;
      case 'e':
        options->swap_end_of_line = 1;
        options->non_printing_char = 1;
        break;
      case 'n':
        options->number_output = 1;
        break;
      case 's':
        options->squeeze_blank = 1;
        break;
      case 't':
        options->show_tabs = 1;
        options->non_printing_char = 1;
        break;
      case 'v':
        options->non_printing_char = 1;
        break;
      case 'T':
        options->show_tabs = 1;
        break;
      case 'E':
        options->swap_end_of_line = 1;
        break;
      default:
        fprintf(stderr, "s21_cat: invalid option --'%c'\n", *c);
        flag = 0;
        break;
      }
    } else {
      break;
    }
  }
  return flag;
}

int OptionParser(CatOptions *options, char *argv) {
  int flag = 1;
  if (*argv == '-') {
    ++argv;
    if (strcmp(argv, "number-nonblank") == 0) {
      options->number_nonempty = 1;
    } else if (strcmp(argv, "number") == 0) {
      options->number_output = 1;
    } else if (strcmp(argv, "squeeze-blank") == 0) {
      options->squeeze_blank = 1;
    } else {
      fprintf(stderr, "s21_cat: unrecognized option '--%s'\n", argv);
      flag = 0;
    }
  } else {
    flag = SingleCharParser(options, argv);
  }
  return flag;
}

int ArgsChecker(CatOptions *options, int argc, char *argv[]) {
  int flag = 1;
  for (int i = 1; i != argc; i++) {
    if (*argv[i] == '-') {
      if (OptionParser(options, ++argv[i]) == 0) {
        options->array_counter = 0;
        flag = 0;
        break;
      }
    } else {
      options->files_index[options->array_counter] = i;
      options->array_counter++;
    }
  }
  return flag;
}

void EmptyArgs() {
  char buffer[ONE_PAGE];
  int bytes_size;

  bytes_size = read(STDIN_FILENO, buffer, ONE_PAGE);
  while (bytes_size > 0) {
    printf("%.*s", bytes_size, buffer);
    bytes_size = read(STDIN_FILENO, buffer, ONE_PAGE);
  }
}

void InitializeStruct(CatOptions *init_opt) {
  init_opt->swap_end_of_line = 0;
  init_opt->number_nonempty = 0;
  init_opt->number_output = 0;
  init_opt->show_tabs = 0;
  init_opt->squeeze_blank = 0;
  init_opt->array_counter = 0;
  init_opt->non_printing_char = 0;
  for (int i = 0; i < ARGV_ARR; i++) {
    init_opt->files_index[i] = '\0';
  }
}
