rz.import("gles20", "gl")
rz.import("debug", "")

GL_COLOR_BUFFER_BIT = 0x00004000
GL_BLEND = 0x0BE2
GL_SRC_ALPHA = 0x0302
GL_ONE = 1
GL_TRIANGLE_STRIP = 0x0005
GL_DEPTH_TEST = 0x0B71

function bind()
    program.bind()
    squircle.bindProgram()
end

function render(w, h)
    gl.glViewport(0, 0, w, h)
    gl.glDisable(GL_DEPTH_TEST)
    gl.glClearColor(0,0,0,1)
    gl.glClear(GL_COLOR_BUFFER_BIT)
    gl.glEnable(GL_BLEND)
    gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE)
    gl.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)
end

function unbind()
    squircle.unbindProgram()
    program.release()
end

debug("loaded squircle.lua")
