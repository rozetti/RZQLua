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
    m_floatsStorage = floats;

    QOpenGLShaderProgram::setAttributeArray(location,
                                            GL_FLOAT,
                                            m_floatsStorage.data(),
                                            tupleSize,
                                            stride);
}
