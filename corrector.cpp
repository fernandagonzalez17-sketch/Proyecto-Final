#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define TAMTOKEN 50
#define MAX_PALABRAS 60000
#define MAX_SUGERENCIAS 6000

int esLetra(char c) {
    if (c >= 'a' && c <= 'z')return 1;
    if (c >= 'A' && c <= 'Z')return 1;
    if (c == 'ñ' || c == 'Ñ')return 1;
    if (c == 'á' || c == 'é' || c == 'í' || c == 'ó' || c == 'ú')return 1;
    if (c == 'Á' || c == 'É' || c == 'Í' || c == 'Ó' || c == 'Ú')return 1;
    return 0;
}

int esNum(char c) {
    return (c >= '0' && c <= '9');
}

int inicioEspecial(char c) {
    return (c == '$' || c == '#' || c == '&');
}

void minus(char* s) {
    for (int i = 0;s[i];i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')s[i] += 32;
        if (s[i] == 'Á')s[i] = 'á';
        if (s[i] == 'É')s[i] = 'é';
        if (s[i] == 'Í')s[i] = 'í';
        if (s[i] == 'Ó')s[i] = 'ó';
        if (s[i] == 'Ú')s[i] = 'ú';
        if (s[i] == 'Ñ')s[i] = 'ñ';
    }
}

void insertarDic(char* palabra, char dic[][TAMTOKEN], int est[], int& n) {
    int l = 0, r = n - 1, mid, c;
    while (l <= r) {
        mid = (l + r) / 2;
        c = strcmp(palabra, dic[mid]);
        if (!c) {
            est[mid]++;
            return;
        }
        if (c > 0)l = mid + 1; else r = mid - 1;
    }
    for (int i = n;i > l;i--) {
        strcpy(dic[i], dic[i - 1]);
        est[i] = est[i - 1];
    }
    strcpy(dic[l], palabra);
    est[l] = 1;
    n++;
}

void Diccionario(char* archivo, char dic[][TAMTOKEN], int est[], int& n) {
    FILE* f = fopen(archivo, "r");
    n = 0;
    if (!f)return;

    char buf[400], tok[TAMTOKEN];
    while (fscanf(f, "%399s", buf) != EOF) {
        int L = strlen(buf);
        int i = 0;

        while (i < L) {
            if (buf[i] == '/' && i + 1 < L && buf[i + 1] == '/') {
                int j = i, k = 0;
                while (j < L && (buf[j] == '/' || esLetra(buf[j]) || esNum(buf[j]))) {
                    if (k < TAMTOKEN - 1) tok[k++] = buf[j];
                    j++;
                }
                tok[k] = '\0';
                minus(tok);
                insertarDic(tok, dic, est, n);
                i = j;
                continue;
            }

            if (inicioEspecial(buf[i])) {
                int j = i, k = 0;
                tok[k++] = buf[j++];
                while (j < L && (esLetra(buf[j]) || esNum(buf[j]))) {
                    if (k < TAMTOKEN - 1) tok[k++] = buf[j];
                    j++;
                }
                tok[k] = '\0';
                minus(tok);
                insertarDic(tok, dic, est, n);
                i = j;
                continue;
            }

            if (esLetra(buf[i]) || esNum(buf[i]) || buf[i] == '-') {
                int j = i, k = 0;
                while (j < L && (esLetra(buf[j]) || esNum(buf[j]) || buf[j] == '-')) {
                    if (k < TAMTOKEN - 1) tok[k++] = buf[j];
                    j++;
                }
                tok[k] = '\0';
                minus(tok);
                insertarDic(tok, dic, est, n);
                i = j;
                continue;
            }

            tok[0] = buf[i];
            tok[1] = '\0';
            minus(tok);
            insertarDic(tok, dic, est, n);
            i++;
        }
    }
    fclose(f);
}

int busquedaBinaria(char* s, char dic[][TAMTOKEN], int n) {
    int l = 0, r = n - 1, mid, c;
    while (l <= r) {
        mid = (l + r) / 2;
        c = strcmp(s, dic[mid]);
        if (!c)return mid;
        if (c > 0)l = mid + 1; else r = mid - 1;
    }
    return -1;
}

void agregar(char* c, char s[][TAMTOKEN], int& n) {
    if (n < MAX_SUGERENCIAS) {
        strcpy(s[n], c);
        n++;
    }
}

void ClonaPalabras(char* p, char sug[][TAMTOKEN], int& ns) {
    ns = 0;
    char t[TAMTOKEN];
    int L = strlen(p);
    char alf[] = "abcdefghijklmnopqrstuvwxyzñáéíóú";
    int A = strlen(alf);

    agregar(p, sug, ns);

    for (int i = 0;i < L;i++) {
        int k = 0;
        for (int j = 0;j < L;j++)if (i != j)t[k++] = p[j];
        t[k] = '\0';
        if (k > 0)agregar(t, sug, ns);
    }

    for (int i = 0;i < L - 1;i++) {
        strcpy(t, p);
        char c = t[i];t[i] = t[i + 1];t[i + 1] = c;
        agregar(t, sug, ns);
    }

    for (int i = 0;i < L;i++) {
        for (int j = 0;j < A;j++) {
            strcpy(t, p);
            t[i] = alf[j];
            agregar(t, sug, ns);
        }
    }

    for (int i = 0;i <= L;i++) {
        for (int j = 0;j < A;j++) {
            int k = 0;
            for (int m = 0;m < i;m++)t[k++] = p[m];
            t[k++] = alf[j];
            for (int m = i;m < L;m++)t[k++] = p[m];
            t[k] = '\0';
            agregar(t, sug, ns);
        }
    }

    char aux[TAMTOKEN];
    for (int i = 0;i < ns - 1;i++) {
        for (int j = 0;j < ns - i - 1;j++) {
            if (strcmp(sug[j], sug[j + 1]) > 0) {
                strcpy(aux, sug[j]);
                strcpy(sug[j], sug[j + 1]);
                strcpy(sug[j + 1], aux);
            }
        }
    }
}

void ListaCandidatas(char sug[][TAMTOKEN], int ns, char dic[][TAMTOKEN], int est[], int nd, char fin[][TAMTOKEN], int peso[], int& nf) {
    nf = 0;
    for (int i = 0;i < ns;i++) {
        int p = busquedaBinaria(sug[i], dic, nd);
        if (p != -1) {
            int ex = 0;
            for (int k = 0;k < nf;k++) {
                if (!strcmp(fin[k], dic[p])) { ex = 1;break; }
            }
            if (!ex) {
                strcpy(fin[nf], dic[p]);
                peso[nf] = est[p];
                nf++;
            }
        }
    }
    char aux[TAMTOKEN];
    int tp;
    for (int i = 0;i < nf - 1;i++) {
        for (int j = 0;j < nf - i - 1;j++) {
            if (peso[j] < peso[j + 1]) {
                strcpy(aux, fin[j]);
                strcpy(fin[j], fin[j + 1]);
                strcpy(fin[j + 1], aux);
                tp = peso[j];
                peso[j] = peso[j + 1];
                peso[j + 1] = tp;
            }
        }
    }
}
