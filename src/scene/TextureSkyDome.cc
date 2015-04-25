#include "TextureSkyDome.h"

TextureSkyDome::TextureSkyDome(int grid, Scene *parent) 
    : SceneObject("Textued SkyDome", "../glsl/skydome.vert", "../glsl/skydome.frag", parent) 
{
    // drawMode = GL_LINE;
    // Load texture
    // QImage decal("../textures/decal_sky.jpg");
    QImage decal("../textures/sky_texture.jpg");
    decalmap.reset(new QOpenGLTexture(decal));
    decalmap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    decalmap->setMagnificationFilter(QOpenGLTexture::Linear);

    // Generate vertex and index data
    {
        float size = 1.0 / grid;
        for (int j = 0; j <= grid; j++) {
            for (int i = 0; i <= grid; i++) {
                vertices << QVector3D(i * size, 0, j * size);
            }
        }
        for (int j = 0; j < grid; j++) {
            for (int i = 0; i < grid; i++) {
                // Upper triangle
                indices << (j * (grid + 1) + i + 1)
                    << (j * (grid + 1) + i) 
                    << ((j + 1) * (grid + 1) + i);

                // Bottom triangle
                indices << ((j + 1) * (grid + 1) + i) 
                    << ((j + 1) * (grid + 1) + i + 1)
                    << (j * (grid + 1) + i + 1); 
            }
        }
    }

}

TextureSkyDome::~TextureSkyDome() {

}

void TextureSkyDome::uniform() {
    // decalmap->bind(0);
    // int decalLocation = program.uniformLocation("uDecalmap");
    // program.setUniformValue(decalLocation, 0);
    

    Camera* camera = dynamic_cast<Scene*>(parent)->getCamera();
    QVector3D cameraPos = camera->getPosition();
    program.setUniformValue("uCenter", cameraPos);

    CHECK_GL_ERROR("after sets uniforms");
}

void TextureSkyDome::update() {
}

void TextureSkyDome::render() {
	glPolygonMode( GL_FRONT_AND_BACK, drawMode );
	CHECK_GL_ERROR("set polygon mode");

    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    SceneObject::render();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
}
