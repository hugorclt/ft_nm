int globalUninitializedVariable;
int globalInitializedVariable = 10;
const int readOnlyVariable = 20;
const static int readOnlyStatic = 5;
static int staticUninitializedVariable;
static int staticInitializedVariable = 10;
int add(int a, int b)
{
        return a+b;
}

int sub(int a, int b);

int main()
{
        static int local_static = 20;
        int local = 20;
        add(local_static, local);
        sub(local_static, local);
}
