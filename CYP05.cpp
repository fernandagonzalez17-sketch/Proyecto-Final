#include <stdio.h>

int main(void) {
    int n;
    scanf_s("%d", &n);

    if (n <= 0) {
        printf("0");
        return 0;
    }

    long long cuentas[2000];
    float calificaciones[2000];
    int count = 0;

    for (int i = 0; i < n; i++) {
        long long cuenta;
        float calificacion;
        scanf_s("%lld %f", &cuenta, &calificacion);

        int encontrado = 0;
        for (int j = 0; j < count; j++) {
            if (cuentas[j] == cuenta) {
                encontrado = 1;
                if (calificacion > calificaciones[j]) {
                    calificaciones[j] = calificacion;
                }
                break;
            }
        }

        if (!encontrado) {
            cuentas[count] = cuenta;
            calificaciones[count] = calificacion;
            count++;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (cuentas[j] > cuentas[j + 1]) {
                long long temp_cuenta = cuentas[j];
                cuentas[j] = cuentas[j + 1];
                cuentas[j + 1] = temp_cuenta;

                float temp_calif = calificaciones[j];
                calificaciones[j] = calificaciones[j + 1];
                calificaciones[j + 1] = temp_calif;
            }
        }
    }

    printf("%d\n", count);
    for (int i = 0; i < count; i++) {
        if (i == count - 1) {
            printf("%lld %f", cuentas[i], calificaciones[i]);
        }
        else {
            printf("%lld %f\n", cuentas[i], calificaciones[i]);
        }
    }

    return 0;
}
