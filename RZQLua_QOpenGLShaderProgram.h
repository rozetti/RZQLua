#pragma once

#include <QtGui/QOpenGLShaderProgram>

class RZQLua_QOpenGLShaderProgram : public QOpenGLShaderProgram
{
public:
    RZQLua_QOpenGLShaderProgram(QObject *parent);

    void setAttributeArrayF(int location, std::vector<float> floats, int tupleSize, int stride = 0);

private:
    std::vector<float> m_floatsStorage;
};
