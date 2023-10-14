
#include <strstream>


class Graphics
{
private:
    std::ostream* outputStream_;

    void init(std::ostream& outputStream);
public:
    Graphics();
    Graphics(std::ostream* outputStream);
    ~Graphics();

    void draw();
};

class GraphicsObject
{

};