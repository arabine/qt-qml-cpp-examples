/** @file Converter.h
 *  @author Anthony Rabine
 *  @copyright Copyright (C) 2018 Anthony Rabine
 *
 * This file is part of the 'DrawSound' program (wav-svg)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

void WavToSvg(const std::string &inputFileName, const std::string &outputFileName);
void SvgToWav(const std::string &inputFileName, const std::string &outputFileName, uint32_t sampleRate, int bitDepth);

#endif // CONVERTER_H
