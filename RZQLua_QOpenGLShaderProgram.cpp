#include "RZQLua_QOpenGLShaderProgram.h"

RZQLua_QOpenGLShaderProgram::RZQLua_QOpenGLShaderProgram(QObject *parent) :
    QOpenGLShaderProgram(parent)
{

}

void RZQLua_QOpenGLShaderProgram::setAttributeArrayF(int location,
                                                     std::vector<float> floats,
                                                     int tupleSize,
                                                     int stride)
{
    m_floats = floats;

    setAttributeArray(location,
                      GL_FLOAT,
                      m_floats.data(),
                      tupleSize,
                      stride);
}
