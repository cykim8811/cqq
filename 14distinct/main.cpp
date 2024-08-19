
char* create_test_case(int n, int pos) {
    // Create a list of n characters, 14 characters from pos is distinct
    char* s = new char[n];
    for (int i = 0; i < n; i++) {
        s[i] = 'a' + (i + pos) % 26;
    }
}