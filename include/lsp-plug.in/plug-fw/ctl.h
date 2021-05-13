/*
 * Copyright (C) 2021 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2021 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-plugin-fw
 * Created on: 10 апр. 2021 г.
 *
 * lsp-plugin-fw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-plugin-fw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-plugin-fw. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSP_PLUG_IN_PLUG_FW_CTL_H_
#define LSP_PLUG_IN_PLUG_FW_CTL_H_

#include <lsp-plug.in/plug-fw/version.h>

#define LSP_PLUG_IN_PLUG_FW_CTL_IMPL_
    #include <lsp-plug.in/plug-fw/ctl/types.h>
    #include <lsp-plug.in/plug-fw/ctl/parse.h>

    // Utilitary classes
    #include <lsp-plug.in/plug-fw/ctl/util/Property.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Integer.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Float.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Boolean.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Expression.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Color.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Embedding.h>
    #include <lsp-plug.in/plug-fw/ctl/util/Padding.h>

    // Widget controllers
    #include <lsp-plug.in/plug-fw/ctl/Widget.h>
    #include <lsp-plug.in/plug-fw/ctl/Factory.h>
    #include <lsp-plug.in/plug-fw/ctl/PluginWindow.h>

    #include <lsp-plug.in/plug-fw/ctl/simple/Void.h>
    #include <lsp-plug.in/plug-fw/ctl/simple/Label.h>
    #include <lsp-plug.in/plug-fw/ctl/simple/Knob.h>
    #include <lsp-plug.in/plug-fw/ctl/simple/Button.h>
    #include <lsp-plug.in/plug-fw/ctl/simple/Led.h>
    #include <lsp-plug.in/plug-fw/ctl/simple/Switch.h>
    #include <lsp-plug.in/plug-fw/ctl/simple/Indicator.h>

    #include <lsp-plug.in/plug-fw/ctl/containers/Box.h>
    #include <lsp-plug.in/plug-fw/ctl/containers/Align.h>
    #include <lsp-plug.in/plug-fw/ctl/containers/Group.h>
    #include <lsp-plug.in/plug-fw/ctl/containers/Grid.h>
    #include <lsp-plug.in/plug-fw/ctl/containers/Cell.h>

    #include <lsp-plug.in/plug-fw/ctl/graph/Graph.h>

#undef LSP_PLUG_IN_PLUG_FW_CTL_IMPL_

#endif /* LSP_PLUG_IN_PLUG_FW_CTL_H_ */
