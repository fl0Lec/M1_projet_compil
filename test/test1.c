//Exemple de programme en DECAF
class Program
{int
compteur,
diviseur
;int
valeurs[4]
;boolean
test;boolean
lecture_valeurs(int
nombre)
// Saisie des valeurs entières
{boolean test;WriteString("Il faut saisir");
WriteInt(nombre);
WriteString("valeurs entières\n");
for i = 0, nombre-1{WriteString("Entrez la valeur");
WriteInt(i+1);WriteString(":\n");ReadInt(valeurs[i]);test
=
valeurs[i] > 0;
// vérification
if
(!test)
{
WriteString("Les valeurs doivent être strictement positives !\n");
return false;}}
return true;}
int moyenne(int
nombre)
{
// Calcul de la moyenne des valeurs
int
somme;for
i
=
0,
nombre-1
{somme
+=
valeurs[i];}return
somme/nombre;}void
main()
{
int nombre;
WriteString("Nombre de valeurs :");
ReadInt(nombre);
if (lecture_valeurs(nombre))
{
WriteString("Moyenne = ");
WriteInt(moyenne(nombre));
WriteString("\n");}else
{
WriteString("Erreur\n");
}}}
