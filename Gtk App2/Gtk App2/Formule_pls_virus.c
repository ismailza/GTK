
//#include "infection.c"
#include "determiner.c"

/// Categorie individu
int cat_ind(Individu ind )
{
   switch(ind.age)
   {
       case 0 - 11:  return 1;
       case 12 - 25: return 2;
       case 26 - 40: return 3;
       case 41 - 65: return 4;
   }
    return 5;
}

/// Letalite du virus selon catégorie d'un individu (Données)
void letalite_cat(gfloat letalite[5],gint id)
{
/// !!!!! Des donnees sans raisonnement exacte

  switch(id)
  {
    /// Corona
  case 1 :
  letalite[0]=20;
  letalite[1]=40;
  letalite[2]=65;
  letalite[3]=80;
  letalite[4]=95;
  break;
  /// Ebola
  case 2 :
  letalite[0]=90;
  letalite[1]=90;
  letalite[2]=90;
  letalite[3]=90;
  letalite[4]=90;
  break;
  ///Rota
  case 3 :
  letalite[0]=95;
  letalite[1]=80;
  letalite[2]=65;
  letalite[3]=40;
  letalite[4]=20;
   break;

  default:
      letalite[0]= (float )g_random_double_range(40,100);
      letalite[1]=(float )g_random_double_range(30,100);
      letalite[2]=(float )g_random_double_range(40,100);
      letalite[3]=(float )g_random_double_range(20,100);
      letalite[4]=(float )g_random_double_range(30,100);
 break;

  }
}

gdouble letalite_total(Individu ind)
{
    gdouble letalite=0.0;
    int i;
    for(i=0;i<4;i++)
    {
        if(ind.etatInd.virus[i].id==0) break;
        letalite+=ind.etatInd.virus[i].letalite[cat_ind(ind)-1];
    }
    letalite=letalite/(gdouble)i;
    return(gdouble) letalite;
}


gdouble dead_or_not(Individu ind )
{
    /// Effet de l'age d'individu
    gdouble eff_age;
     eff_age= ((gdouble)cat_ind(ind))/5.0;

    /// Effet du bilan d'individu
    gdouble eff_sante;
    eff_sante=((ind.bl.cr/3.0)*0.2)+((ind.bl.db/3.0)*0.2)+((ind.bl.gn/4.0)*0.2)+((ind.bl.pm/3.0)*0.2)+((ind.bl.ta/4.0)*0.2);
    ///Interaction des deux
    gdouble effet=(eff_sante*0.5)+(eff_age*0.5);
    printf(" \n ---->  Wa mooot ..... : %.2f %% \n",effet*letalite_total(ind));
    /// Effet total avec la létalité du virus etudié selon la categorie
    return (float) effet*letalite_total(ind);

}
