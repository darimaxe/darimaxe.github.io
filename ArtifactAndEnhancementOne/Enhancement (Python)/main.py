import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *

verticies = (
    (0, -2, -1),
    (2, -2, -1),
    (2, 4, -1),
    (0, 4, -1),

    (0, -2, 1),
    (2, -2, 1),
    (0, 4, 1),
    (2, 4, 1),


    (0, 4, 1),
    (8, 4, 1),
    (8, 5, 1),
    (0, 5, 1),

    (0, 4, -6),
    (8, 4, -6),
    (0, 5, -6),
    (8, 5, -6),


    (6, -2, 1),
    (8, -2, 1),
    (8, 4, 1),
    (6, 4, 1),

    (6, -2, -1),
    (8, -2, -1),
    (6, 4, -1),
    (8, 4, -1),


    (0, -2, -6),
    (2, -2, -6),
    (2, 4, -6),
    (0, 4, -6),

    (0, -2, -4),
    (2, -2, -4),
    (0, 4, -4),
    (2, 4, -4),


    (6, -2, -4),
    (8, -2, -4),
    (8, 4, -4),
    (6, 4, -4),

    (6, -2, -6),
    (8, -2, -6),
    (6, 4, -6),
    (8, 4, -6),


    (0, 5, -4),
    (8, 5, -4),
    (8, 12, -4),
    (0, 12, -4),

    (0, 5, -6),
    (8, 5, -6),
    (0, 12, -6),
    (8, 12, -6),
)

edges = (
    (0, 1),
    (0, 3),
    (0, 4),
    (2, 1),
    (2, 3),
    (2, 7),
    (6, 3),
    (6, 4),
    (6, 7),
    (5, 1),
    (5, 4),
    (5, 7),

    (8, 9),
    (8, 11),
    (8, 12),
    (10, 9),
    (10, 11),
    (10, 15),
    (14, 11),
    (14, 12),
    (14, 15),
    (13, 9),
    (13, 12),
    (13, 15),

    (16, 17),
    (16, 19),
    (16, 20),
    (18, 17),
    (18, 19),
    (18, 23),
    (22, 19),
    (22, 20),
    (22, 23),
    (21, 17),
    (21, 20),
    (21, 23),

    (24, 25),
    (24, 27),
    (24, 28),
    (26, 25),
    (26, 27),
    (26, 31),
    (30, 27),
    (30, 28),
    (30, 31),
    (29, 25),
    (29, 28),
    (29, 31),

    (32, 33),
    (32, 35),
    (32, 36),
    (34, 33),
    (34, 35),
    (34, 39),
    (38, 35),
    (38, 36),
    (38, 39),
    (37, 33),
    (37, 36),
    (37, 39),

    (40, 41),
    (40, 43),
    (40, 44),
    (42, 41),
    (42, 43),
    (42, 47),
    (46, 43),
    (46, 44),
    (46, 47),
    (45, 41),
    (45, 44),
    (45, 47),
)


surfaces = (
    (0, 1, 2, 3),
    (3, 2, 7, 6),
    (6, 7, 5, 4),
    (4, 5, 1, 0),
    (1, 5, 7, 2),
    (4, 0, 3, 6),

    (8, 9, 10, 11),
    (11, 10, 15, 14),
    (14, 15, 13, 12),
    (12, 13, 9, 8),
    (9, 13, 15, 10),
    (12, 8, 3, 6),

    (16, 17, 18, 19),
    (19, 18, 23, 22),
    (22, 23, 21, 20),
    (20, 21, 17, 16),
    (17, 21, 23, 18),
    (20, 16, 19, 22),

    (24, 25, 26, 27),
    (27, 26, 31, 30),
    (30, 31, 29, 28),
    (28, 29, 25, 24),
    (25, 29, 31, 26),
    (28, 24, 27, 30),

    (32, 33, 34, 35),
    (35, 34, 39, 38),
    (38, 39, 37, 36),
    (36, 37, 33, 32),
    (33, 37, 39, 34),
    (36, 32, 35, 38),

    (40, 41, 42, 43),
    (43, 42, 47, 46),
    (46, 47, 45, 44),
    (44, 45, 41, 40),
    (41, 45, 47, 42),
    (44, 40, 43, 46)
)

colors = (

    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (1, 0.156, 0),
    (1, 0.156, 0),
    (1, 0.156, 0),
    (1, 0.156, 0),
    (1, 0.156, 0),

    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),

    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),

    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),

    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),

    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
    (0.396, 0.263, 0.129),
)

def Chair():
    glBegin(GL_QUADS)
    x = 0
    for surface in surfaces:
        x += 1
        for vertex in surface:
            glColor3fv(colors[x])
            glVertex3fv(verticies[vertex])
    glEnd()

    glBegin(GL_LINES)
    for edge in edges:
        for vertex in edge:
            glVertex3fv(verticies[vertex]) #function wants a vertex so we are going to draw some lines
    glEnd()

def main():
    pygame.init() #initialize pygame
    display = (800, 600)
    center = 800 / 2, 600 / 2
    pygame.display.set_mode(display, DOUBLEBUF|OPENGL) #double buffer what you are seeing and what is in background and using opengl
    #^sets display
    gluPerspective(45, (display[0] / display[1]), 0.1, 50.0) #45 degrees, , clips away the showing of the object

    glTranslatef(0, 0, -40) #x, y, z our relation to the object

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        glRotatef(1, 5, 5, 0)
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                glTranslatef(-0.5, 0, 0)  # move left
            if event.key == pygame.K_RIGHT:
                glTranslatef(0.5, 0, 0)  # move right
            if event.key == pygame.K_UP:
                glTranslatef(0, 0.5, 0)  # move up
            if event.key == pygame.K_DOWN:
                glTranslatef(0, -0.5, 0)  # move down

        if event.type == MOUSEMOTION:
            mouse = event.pos
            x = mouse[0] - center[0]
            y = mouse[1] - center[1]

            sensitivity = 0.05
            x = x * sensitivity
            y = y * sensitivity
            glRotatef(1, x, y, 0)  # by degrees


        #glRotatef(1, 3, 1, 1)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) #clear the frame with a fill, what we want to clear explicitly

        Chair()
        pygame.display.flip()
        pygame.time.wait(10) #10 ms

main()