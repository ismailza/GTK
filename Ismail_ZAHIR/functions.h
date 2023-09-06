
void epurer_blan(FILE* f1)
{
    char c;
    while ((c = fgetc(f1)) == ' ');
    ungetc(c, f1);
}

typedef struct lsA
{
    gchar attribut[50];
    gchar valeur[50];
    struct lsA* suiv;
}obj_att;

obj_att* creer_initialiser_obj_att()
{
    obj_att* ne = (obj_att*)calloc(1, sizeof(obj_att));
    if (!ne)
    {
        printf("\nErreur");
        exit(0);
    }
    ne->suiv = NULL;
    return (obj_att*)ne;
}

obj_att* inserer_obj_att(gchar att[50], gchar val[50], obj_att* info)
{
    obj_att* ne = creer_initialiser_obj_att();
    strcpy(ne->attribut, att);
    strcpy(ne->valeur, val);
    if (info) ne->suiv = info;
    info = ne;
    return ((obj_att*)info);
}

void str_to_val1_val2(gchar* in, int* val1, int* val2)
{
    int i = 0, val = 0, k;
    while (in[i] != '\0' && in[i] != '|')
    {
        k = (int)in[i] - (int)'0';
        val = val * 10 + k;
        i++;
    }
    *val1 = val;
    val = 0;
    while (in[i] != '\0')
    {
        k = (int)in[i] - (int)'0';
        val = val * 10 + k;
        i++;
    }
    *val2 = val;
}

obj_att* lire_att(FILE* f1)
{
    char c;
    obj_att* liste = NULL;
    gchar mot[50] = { 0 };
    gchar mot1[50] = { 0 };
    c = (char)fgetc(f1);
    if (c == '>')
    {
        fscanf(f1, "\n");
        epurer_blan(f1);
        return (obj_att*)NULL;
    }
    do {
        ungetc(c, f1);
        fscanf(f1, "%50[^=]=%50[^,],", mot, mot1);
        liste = inserer_obj_att(mot, mot1, liste);
        c = (char)fgetc(f1);
    } while (c != '>');
    fscanf(f1, "\n");
    epurer_blan(f1);
    return ((obj_att*)liste);
}