#ifndef CREDITS_GREETS_H_INCLUDED
#define CREDITS_GREETS_H_INCLUDED

#include "glew.h"
#include <GLFW/glfw3.h>

#include "Variables.h"
#include "fonctions.h"
#include "math.h"
#include "Interpolations.h"

class Credits_Greets
{
    public:
        static bool drapeau_init_greets;
        static bool drapeau_init_credits;
        static float t0;
    public:
        static void affiche_credits();
        static void affiche_greets();

};
#endif // CREDITS_GREETS_H_INCLUDED
