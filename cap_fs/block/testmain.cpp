#include "padded_block_file.hpp"

class dummy {
  public:
     dummy(const char *key){}
};

int main(int argc,char **argv) {
    umask(0007);
    padded_blockfile<dummy> bf1("/tmp/test_file","keykeykeyKEYKEYkeykeykeyKEYKEY--",data_file);
    padded_blockfile<dummy> bf2("/tmp/test_exe","keykeykeyKEYKEYkeykeykeyKEYKEY--",exe_file);
    padded_blockfile<dummy> bf3("/tmp/test_dir","keykeykeyKEYKEYkeykeykeyKEYKEY--",directory);
    padded_blockfile<dummy> bf4("/tmp/test_link","keykeykeyKEYKEYkeykeykeyKEYKEY--",symbolic_link);
    padded_blockfile<dummy> bf5("/path/leidt/naar/nergens","keykeykeyKEYKEYkeykeykeyKEYKEY--",symbolic_link);
}
