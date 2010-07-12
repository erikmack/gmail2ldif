#ifndef INPUT_H_
#define INPUT_H_

void set_input( int fd );
int has_more_wchars();
int can_peek();
wchar_t get_current_char();
wchar_t peek_next_char();

void input_destroy();
int  input_initialize();

#endif // INPUT_H_
