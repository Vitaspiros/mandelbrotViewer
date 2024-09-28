#ifndef SHADER_H
#define SHADER_H

class Shader {
    public:
        Shader(const char*, int);
        bool compile();
        void dispose();
        void attach(unsigned int);

        static bool linkProgram(unsigned int);
    private:
        unsigned int shaderId;
};

#endif