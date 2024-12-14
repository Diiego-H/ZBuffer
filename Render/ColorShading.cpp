#include "ColorShading.hh"


vec3 ColorShading::shading(shared_ptr<Scene> scene, vector<shared_ptr<HitRecord>> infos, vec3 lookFrom, bool shadow) {
    if (!infos.empty()) {
            auto m = infos[0]->mat;
            return m->Kd;
        } else {
            // Tracta la situació on el vector infos és buit
            // Retorna un valor predeterminat o maneja-ho segons les teves necessitats.
            return vec3(0.0f);
        }
}
