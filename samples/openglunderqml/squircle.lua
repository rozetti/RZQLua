rz.import("gles20", "gl")
rz.import("debug", "")

GL_COLOR_BUFFER_BIT = 0x00004000
GL_BLEND = 0x0BE2
GL_SRC_ALPHA = 0x0302
GL_ONE = 1
GL_TRIANGLE_STRIP = 0x0005
GL_DEPTH_TEST = 0x0B71
GL_FLOAT = 0x1406

m_t = 0;

vertex_shader = [[
attribute highp vec4 vertices;
varying highp vec2 coords;
void main()
{
    gl_Position = vertices; coords = vertices.xy;
}
]]

fragment_shader = [[
uniform lowp float t;
varying highp vec2 coords;
void main()
{
    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));
    i = smoothstep(t - 0.8, t + 0.8, i);
    i = floor(i * 20.) / 20.;
    gl_FragColor = vec4(coords * .5 + .5, i, i);
}
]]

function create_program()
    program = new("QOpenGLShaderProgram")
    program.addShaderFromSourceCode(1, vertex_shader)
    program.addShaderFromSourceCode(2, fragment_shader)
    program.bindAttributeLocation("vertices", 0)
    program.link()
end

vertices = { -1, -1, 1, -1, -1, 1, 1, 1 }

function bind()
    program.bind()
    program.enableAttributeArray(0)
    program.setAttributeArrayF(0, vertices, 2)
    program.setUniformValueF("t", m_t)
end

function update(t)
    if nil == program then
        create_program()
    end

    m_t = t
end

function render(w, h)
    bind()

    gl.glViewport(0, 0, w, h)
    gl.glDisable(GL_DEPTH_TEST)
    gl.glClearColor(0,0,0,1)
    gl.glClear(GL_COLOR_BUFFER_BIT)
    gl.glEnable(GL_BLEND)
    gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE)
    gl.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)

    unbind()
end

function unbind()
    program.disableAttributeArray(0)
    program.release()
end

debug("loaded squircle.lua")
