#ifndef UTLOP_WINDOW
#define UTLOP_WINDOW 1

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

#endif // !UTLOP_WINDOW
