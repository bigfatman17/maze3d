/* Maze 3D - A simplistic random maze generator.
 * Copyright (C) 2016 Armend Murtishi

 * This file is part of Maze 3D.

 * Maze 3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Maze 3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Maze 3D.  If not, see <http://www.gnu.org/licenses/>.
 */

#version 330 core

in vec3 norm;
in vec3 fragPos;

out vec4 color;

uniform vec3 light;
uniform vec3 lightPos;

void main()
{
    vec3 ambientLight = 0.2f * light;

    vec3 lightDirection = normalize(lightPos - fragPos);
    // note: angle is actually cosined!
    float angle = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = angle * light;

    color = vec4((ambientLight + diffuse) * vec3(1, 1, 1), 1);
}
