#ifndef Utlop_WINDOW
#define Utlop_WINDOW 1

struct GLFWwindow;

namespace Utlop
{

  class Window
  {
    public:

      //  Inits the window with a specified size
      void init(int width, int height, char* title);

      friend class Core;
    protected:

    private:
      Window();
      ~Window();

      bool isValid();

      GLFWwindow* _window;
  };


}

#endif // !Utlop_WINDOW
