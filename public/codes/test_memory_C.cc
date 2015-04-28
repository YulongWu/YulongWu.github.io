#include <stdio.h>
#include <stdlib.h>

static int gv21 = 1;
int gv22 = 1;
int main() {
    int *max_width = 46;
    char* v1 = "a";
    printf("                  --------Code Area--------\n");
    printf("&%*s = %p\n", max_width, "string_literal", v1);
    char *v11 = "b";
    printf("$%*s = %p\n", max_width, "another_string_literal", v11);
    char *v12 = "b";
    printf("$%*s = %p\n", max_width, "another_string_literal_with_same_value(pooled)", v12);
    printf("                  --------Global Area--------\n");
    const static int v31 = 1;
    printf("$%*s = %p\n", max_width, "const_static_variable", &v31);
    static int v2 = 1;
    printf("$%*s = %p\n", max_width, "static_variable", &v2);
    printf("$%*s = %p\n", max_width, "global_static_variable", &gv21);
    printf("$%*s = %p\n", max_width, "global_variable", &gv22);
    printf("                  --------Heap Area--------\n");
    int *v4 = (int *)malloc(sizeof(int));
    printf("$%*s = %p\n", max_width, "malloc_variable", v4);
    int *v5 = new int(1);
    printf("$%*s = %p\n", max_width, "new_variable", v5);
    printf("                  --------Heap Area--------\n");
    const int v3 = 1;
    printf("$%*s = %p\n", max_width, "const_variable", &v3);
    int v6 = 1;
    printf("$%*s = %p\n", max_width, "local_variable", &v6);
}
