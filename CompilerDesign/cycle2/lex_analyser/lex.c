#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 100
const char *keywords[] = {"if",  "else",  "while", "return",
                          "int", "float", "void"};
const int keywordCount = sizeof(keywords) / sizeof(keywords[0]);

int isKeyword(char *lexeme) {
  if (lexeme == NULL)
    return 0;
  for (int i = 0; i < keywordCount; i++) {
    if (strcmp(lexeme, keywords[i]) == 0)
      return 1;
  }
  return 0;
}

int isDelimiter(char ch) {
  return (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' ||
          ch == '}' || ch == '[' || ch == ']');
}

int isOperator(char ch) {
  return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' ||
          ch == '<' || ch == '>' || ch == '!' || ch == '&' || ch == '|' ||
          ch == '%' || ch == '^' || ch == '~' || ch == '?' || ch == ':');
}
void skipSingleLineComment(FILE *fp) {
  int ch;
  while ((ch = fgetc(fp)) != EOF && ch != '\n') {
  } // skipping comment
  if (ch == '\n') {
    ungetc(ch, fp);
  }
}
void skipMultiLineComment(FILE *fp) {
  int ch, prev = 0;
  while ((ch = fgetc(fp)) != EOF) {
    if (prev == '*' && ch == '/')
      break;
    prev = ch;
  }
}

void lexer(FILE *fp, FILE *op) {
  int c, tokenCount = 0, i;
  char buffer[MAX_LEN];
  while ((c = fgetc(fp)) != EOF) {
    if (isspace(c))
      continue; // skip if whitespace (ctype library function)

    // KEYWORD OR IDENTIFIER
    if (isalpha(c) || c == '_') { // start of identifier or keyword
      i = 0;
      buffer[i++] = c;
      while (i < MAX_LEN - 1 && (isalnum(c = fgetc(fp)) || c == '_')) {
        buffer[i++] = c;
      }
      ungetc(c, fp); // put back the last read character
      buffer[i] = '\0';
      if (isKeyword(buffer)) {
        fprintf(op, "<KEYWORD, %s>\n", buffer);
      } else {
        fprintf(op, "<IDENTIFIER, %s>\n", buffer);
      }
      tokenCount++;
    }
    // NUMBER
    else if (isdigit(c)) {
      i = 0;
      int hasDot = 0;
      buffer[i++] = c;
      while (i < MAX_LEN - 1 && (c = fgetc(fp)) != EOF) {
        if (isdigit(c)) {
          buffer[i++] = c;
        } else if (c == '.' && !hasDot) {
          hasDot = 1;
          buffer[i++] = c;
        } else {
          ungetc(c, fp);
          break;
        }
      }
      buffer[i] = '\0';
      fprintf(op, "<NUMBER, %s>\n", buffer);
      tokenCount++;
    }
    // STRING
    else if (c == '"') {
      i = 0;
      buffer[i++] = c;
      while (i < MAX_LEN - 1 && (c = fgetc(fp)) != EOF) {
        buffer[i++] = c;
        if (c == '"')
          break;
        if (c == '\\') {
          if (i < MAX_LEN - 1 && (c = fgetc(fp)) != EOF) {
            buffer[i++] = c;
          }
        }
      }
      buffer[i] = '\0';
      fprintf(op, "<STRING, %s>\n", buffer);
      tokenCount++;
    }
    // CHARACTER
    else if (c == '\'') {
      i = 0;
      buffer[i++] = c;
      while (i < MAX_LEN - 1 && (c = fgetc(fp)) != EOF) {
        buffer[i++] = c;
        if (c == '\'')
          break;
        if (c == '\\') {
          if (i < MAX_LEN - 1 && (c = fgetc(fp)) != EOF) {
            buffer[i++] = c;
          }
        }
      }
      buffer[i] = '\0';
      fprintf(op, "<CHARACTER, %s>\n", buffer);
      tokenCount++;
    }

    // OPERATOR
    else if (isOperator(c)) {
      int next = fgetc(fp);
      if (next != EOF) {

        if ((c == '=' && next == '=') || (c == '!' && next == '=') ||
            (c == '<' && next == '=') || (c == '>' && next == '=') ||
            (c == '&' && next == '&') || (c == '|' && next == '|') ||
            (c == '+' && next == '+') || (c == '-' && next == '-') ||
            (c == '+' && next == '=') || (c == '-' && next == '=') ||
            (c == '*' && next == '=') || (c == '/' && next == '=') ||
            (c == '%' && next == '=') || (c == '<' && next == '<') ||
            (c == '>' && next == '>') || (c == '-' && next == '>')) {
          fprintf(op, "<OPERATOR, %c%c>\n", c, next);
        } else {
          ungetc(next, fp);
          fprintf(op, "<OPERATOR, %c>\n", c);
        }
      } else {
        fprintf(op, "<OPERATOR, %c>\n", c);
      }
      tokenCount++;
    }

    // Delimiters/Punctuation
    else if (isDelimiter(c)) {
      fprintf(op, "<DELIMITER, %c>\n", c);
      tokenCount++;
    }

    // Preprocessor directives
    else if (c == '#') {
      i = 0;
      buffer[i++] = c;

      while (i < MAX_LEN - 1 && (c = fgetc(fp)) != EOF) {
        if (c == '\n') {
          ungetc(c, fp);
          break;
        }
        buffer[i++] = c;
      }

      buffer[i] = '\0';
      fprintf(op, "<PREPROCESSOR, %s>\n", buffer);
      tokenCount++;
    }
  }
  printf("Total tokens: %d\n", tokenCount);
}
int main() {
  FILE *fp = fopen("code.txt", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return 1;
  }
  FILE *op = fopen("output.txt", "w");
  lexer(fp, op);
  fclose(fp);
  fclose(op);
  return 0;
}
