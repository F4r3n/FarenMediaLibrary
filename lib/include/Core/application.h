#ifndef APPLICATION_H
#define APPLICATION_H


namespace fm
{
class Application
{
    public:
        Application();
        ~Application();

        bool Serialize();
        bool Read();

        void Start();
        void Init();
};
}

#endif // APPLICATION_H

