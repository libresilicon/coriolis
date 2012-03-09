#!/usr/bin/python

# This file is part of the Coriolis Project.
# Copyright (C) Laboratoire LIP6 - Departement ASIM
# Universite Pierre et Marie Curie
#
# Main contributors :
#        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
#        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
#        Hugo Clement                   <Hugo.Clement@lip6.fr>
#        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
#        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
#        Christian Masson           <Christian.Masson@lip6.fr>
#        Marek Sroka                     <Marek.Sroka@lip6.fr>
# 
# The  Coriolis Project  is  free software;  you  can redistribute  it
# and/or modify it under the  terms of the GNU General Public License
# as published by  the Free Software Foundation; either  version 2 of
# the License, or (at your option) any later version.
# 
# The  Coriolis Project is  distributed in  the hope  that it  will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
# GNU General Public License for more details.
# 
# You should have  received a copy of the  GNU General Public License
# along with the Coriolis Project;  if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA
#
# License-Tag
# Authors-Tag
# ===================================================================
#
# x-----------------------------------------------------------------x 
# |                                                                 |
# |                   C O R I O L I S                               |
# |        S t r a t u s   -  Netlists/Layouts Description          |
# |                                                                 |
# |  Author      :                    Sophie BELLOEIL               |
# |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
# | =============================================================== |
# |  Py Module   :       "./stratus.py"                             |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x


# Get configuration of Stratus
def getConfigFile():
   import os

   config_file = os.path.join(os.getcwd(),".st_config.py")
   if os.path.exists(config_file):
      return config_file
   else:
      config_file = os.path.join(os.getenv('HOME'),".st_config.py")
      if os.path.exists(config_file):
         return config_file
      else:
         return None

configFile = getConfigFile()
if configFile:
   import imp
   imp.load_source('st_config',configFile)
else:
   print "No configuration file found, using default configuration"
   import st_config


from st_model         import *
from st_net           import *
from st_instance      import *
from st_placement     import *
from st_placeAndRoute import *
from st_ref           import *
from st_generate      import *
from st_const         import *
from st_cat           import *
from st_param         import *
from st_getrealmodel  import GetWeightTime, GetWeightArea, GetWeightPower

from util_Const       import *
from util_Defs        import *
from util_Misc        import *
from util_Gen         import *
from util_Shift       import *
from util_uRom        import *
from util             import *

from patterns         import *



