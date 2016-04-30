rz.import("debug")
rz.import("QOpenGLFunctions")
rz.import("QOpenGLShaderProgram")

gl = QOpenGLFunctions()

GL_COLOR_BUFFER_BIT = 0x00004000
GL_BLEND = 0x0BE2
GL_SRC_ALPHA = 0x0302
GL_ONE = 1
GL_TRIANGLE_STRIP = 0x0005
GL_DEPTH_TEST = 0x0B71
GL_FLOAT = 0x1406

parameter = 0;
vertices = { -1, -1, 1, -1, -1, 1, 1, 1 }

vs = [[
attribute highp vec4 vertices;
varying highp vec2 coords;
void main()
{
    gl_Position = vertices;
    coords = vertices.xy;
}
]]

fs_1 = [[
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

fs_2 = [[
uniform lowp float t;
varying highp vec2 coords;
void main()
{
    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));
    i = smoothstep(t - 0.8, t + 0.8, i);
    i = floor(i * 20.) / 20.;
    gl_FragColor = vec4(coords * .5 + .5, i + 2. * (t - .5), i + t - .5);
}
]]

function create_program(vertex_shader, fragment_shader)
    local program = QOpenGLShaderProgram()

    debug(program.instanceName)

    program.addShaderFromSourceCode(1, vertex_shader)
    program.addShaderFromSourceCode(2, fragment_shader)
    program.bindAttributeLocation("vertices", 0)
    program.link()

    return program
end

function create_programs()
    program_1 = create_program(vs, fs_1)
    program_2 = create_program(vs, fs_2)
    current_program = program_1
end

function bind()
    current_program.bind()
    current_program.enableAttributeArray(0)
    current_program.setAttributeArrayF(0, vertices, 2)
    current_program.setUniformValueF("t", parameter)
end

function unbind()
    current_program.disableAttributeArray(0)
    current_program.release()
end

function update(t)
    if nil == program_1 then
        create_programs()
    end

    parameter = t

    if parameter < 0.5 then
        current_program = program_1
    else
        current_program = program_2
    end
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

debug("loaded squircle.lua")
