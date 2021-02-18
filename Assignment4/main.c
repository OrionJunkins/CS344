#include "line_processor.c"

int main () {
    pthread_t in, sep, proc, out;

    

    pthread_create(&in, NULL, input_thread, NULL);
    pthread_create(&sep, NULL, line_separator_thread, NULL);
    pthread_create(&proc, NULL, plus_signal_thread, NULL);
    pthread_create(&out, NULL, output_thread, NULL);


    pthread_join(in, NULL);
    pthread_join(sep, NULL);
    pthread_join(proc, NULL);
    pthread_join(out, NULL);
    return 0;
}