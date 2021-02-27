/*
   SPDX-FileCopyrightText: 2021 Dan Leinir Turthra Jensen <admin@leinir.dk>

   SPDX-License-Identifier: LGPL-2.0-only
*/

#ifndef COLORSAPPLICATOR_H
#define COLORSAPPLICATOR_H

class ColorsSettings;
class ColorsModel;
/**
 * Performs the task of actually applying a color scheme to the current session, based on
 * what is currently set in the settings and model instances passed into the function.
 * When using this function, you select the scheme to use by setting the model's selected scheme
 * @param settings The settings instance which lets us update the system with the new colors
 * @param model The model which holds the information on which scheme is currently selected, and what colors it contains
 * @see ColorsModel::setSelectedScheme(QString)
 */
void applyScheme(ColorsSettings *settings, ColorsModel *model);

#endif//COLORSAPPLICATOR_H
