#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <thread>
#include <chrono>

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include <list>

struct In_use_Words{

      int col_number;
      std::string *Word;

};

class Terminal_matrix
{

    private:
      
      int lines_size, cols_size;
      std::vector<std::vector<char>> Matrix_Char; 
      std::list<std::string> Words_Buffer;
      std::vector<int> Available_col_number;
      std::vector<char> new_line;
      std::vector<In_use_Words> In_use_Words_Buffer;
      


    public:

      Terminal_matrix(){

            // Get the size of the terminal 
            this->refresh_terminal_size();

            // Fill the Matrix_Char 2D array with the spaces
            for(int i = 0; i < this->lines_size; i++){

                  std::vector<char> actual_line(this->cols_size, ' ');
                  this->Matrix_Char.push_back(actual_line);

            }

            // Fill the Available_col_number list with number from 0 to cols_size
            for(int i = 0; i < this->cols_size; i++){

                  this->Available_col_number.push_back(i);
            }
      }

      void ACTION(){

            this->refresh_in_use_words_buffer();
            this->set_new_line();
            this->print_Matrix();
            
      }

      void add_word_to_buffer(std::string new_word){
            this->Words_Buffer.push_back(new_word);
      }

      void set_new_line(){

            // Clean the new line
            this->new_line = std::vector<char>(this->cols_size, ' ');

            for(int it = 0; it < this->In_use_Words_Buffer.size(); it++){

                  In_use_Words element = In_use_Words_Buffer[it];

                  // If the word size is 0, all the chars have been shown
                  // the col position can now be available again.
                  if(element.Word->size() == 0){
                        
                        // delete element.Word;
                        this->Available_col_number.push_back(element.col_number);
                  // Set the new_line chars
                  }else{
                        
                        int word_size = element.Word->size();
                        char new_char = element.Word->at(word_size - 1);
                        element.Word->resize(word_size - 1);

                        this->new_line[element.col_number] = new_char;

                  }
            }
      }

      void refresh_in_use_words_buffer(){

            // If there are no colunms available --> return
            if(this->Available_col_number.size() == 0){
                  return;
            }

            // If the buffer with the words is empty -> return
            // TODO: When the buffer is empty, generate a random word.
            if(this->Words_Buffer.size() == 0){
                  
                  std::string random_string;
                  int length = rand() % 6 + 4;
                  for(int it = 0; it < length; it++){
                        
                        int test = rand() % 3;
                        // test = 2;
                        int iSecret = 0;
                        if(test == 0){
                              iSecret = rand() % 25 + 97;
                        }else if(test == 1){
                              iSecret = rand() % 10 + 48;
                        }else{
                              iSecret = rand() % 25 + 65;
                        }
                        
                        if(it == length - 1){
                              iSecret = 32;
                        }

                        // int iSecret = 97;
                        char c = iSecret;

                        random_string += c;
                  }

                  // this->Words_Buffer.push_back(" NEW_word");
                  this->Words_Buffer.push_back(random_string);

                  // return;
            }

            /* Generate a random number col (0 - size of available positions) from the matrix to print the word */
            int position = rand() % this->Available_col_number.size();

            // New Word to appear on the Matrix rain
            In_use_Words new_word;
            // Setting the new word params
            new_word.col_number = this->Available_col_number[position];
            new_word.Word = new std::string(this->Words_Buffer.back());

            // Remove the last word from the buffer
            this->Words_Buffer.pop_back();
            // Remove the col number from the list of available positions on Matrix
            this->Available_col_number.erase(this->Available_col_number.begin() + position);

            // Adds the new word to the In_use_Words_Buffer
            this->In_use_Words_Buffer.push_back(new_word);

      }

      void Move_letters_down(){

            for(int it = this->lines_size; it > 0 ; it--){
                  this->Matrix_Char[it] = this->Matrix_Char[it - 1];
            }

            this->Matrix_Char[0] = this->new_line;

      }
      void print_Matrix(){
            
            for(auto it : this->Matrix_Char){
                  for(char c : it){
                        std::cout << c;

                  }
            }
            std::cout << std::endl;

            this->Move_letters_down();
      }

      void refresh_terminal_size()
      {

            this->lines_size = 0;
            this->cols_size = 0;

            #ifdef TIOCGSIZE
                  struct ttysize ts;
                  ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
                  this->cols_size = ts.ts_cols;
                  this->lines_size = ts.ts_lines - 1;
            #elif defined(TIOCGWINSZ)
                  struct winsize ts;
                  ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
                  this->cols_size = ts.ws_col;
                  this->lines_size = ts.ws_row;
            #endif /* TIOCGSIZE */
      }
};

int main()
{

      /* initialize random seed: */
      srand(time(NULL));
      Terminal_matrix T_matrix;

      while(1){
            
            T_matrix.ACTION();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      

      // while (1)
      // {

      //       /* generate secret number between 1 and 10: */
      //       int iSecret = rand() % 126 + 33;

      //       char c = iSecret;
      //       std::cout << c << std::endl;
      //       std::this_thread::sleep_for(std::chrono::milliseconds(200));
      //       test_size();
      // }
      return 0;
}