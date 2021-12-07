from typing import Callable
import bpy

# HOW TO RUN: open in blender using built-in "Text Editor" or built-in "Python Console"
# /\ will resolve bpy and auto-import currently opened blender project as bpy.context

# DON'T OPEN IN NORMAL TERMINAL, or you will get errors because of missing context

class Obj:
    def _getSortableId_(self):
        intId = -1
        try:
            intId = int(self.id)
        except:
            intId = -1
        return intId

    def __init__(self,  _id:str = "X", sceneIds = ["X"], path = "", _type = "", name = "",
                        pos_x:float = 0.0, pos_y:float = 0.0, pos_z:float = 0.0,
                        rot_x:float = 0.0, rot_y:float = 0.0, rot_z:float = 0.0,
                        scale_x:float = 0.0, scale_y:float = 0.0, scale_z:float = 0.0,
                        dim_x:float = 0.0, dim_y:float = 0.0, dim_z:float = 0.0):
        self.id = _id
        self.sortableId = self._getSortableId_()
        self.sceneIds = sceneIds
        self.path = path
        self.name = name
        self.type = _type
        self.precision = 17 # blender default, also python maximal float precision
        
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.pos_z = pos_z
        
        # xyz euler, but can also export quaternion if needed
        self.rot_x = rot_x
        self.rot_y = rot_y
        self.rot_z = rot_z
        
        self.scale_x = scale_x
        self.scale_y = scale_y
        self.scale_z = scale_z
        
        self.dim_x = dim_x
        self.dim_y = dim_y
        self.dim_z = dim_z

        return
    
    def normalize(self, max_pos:float, max_rot:float, max_scale:float, max_dim:float, precision:int = 17):
        self.precision = precision

        self.pos_x = self.pos_x / max_pos
        self.pos_y = self.pos_y / max_pos
        self.pos_z = self.pos_z / max_pos
        
        self.rot_x = self.rot_x / max_rot
        self.rot_y = self.rot_y / max_rot
        self.rot_z = self.rot_z / max_rot
        
        self.scale_x = self.scale_x / max_scale
        self.scale_y = self.scale_y / max_scale
        self.scale_z = self.scale_z / max_scale
        
        self.dim_x = self.dim_x / max_dim
        self.dim_y = self.dim_y / max_dim
        self.dim_z = self.dim_z / max_dim
        return


    def __str__(self):
        return  self.id + " " + \
                format(self.pos_x, "." + str(self.precision) + "f") + " " + format(self.pos_y, "." + str(self.precision) + "f") + " " + format(self.pos_z, "." + str(self.precision) + "f") + " " + \
                format(self.rot_x, "." + str(self.precision) + "f") + " " + format(self.rot_y, "." + str(self.precision) + "f") + " " + format(self.rot_z, "." + str(self.precision) + "f") + " " + \
                format(self.scale_x, "." + str(self.precision) + "f") + " " + format(self.scale_y, "." + str(self.precision) + "f") + " " + format(self.scale_z, "." + str(self.precision) + "f") + " " + \
                format(self.dim_x, "." + str(self.precision) + "f") + " " + format(self.dim_y, "." + str(self.precision) + "f") + " " + format(self.dim_z, "." + str(self.precision) + "f")

class ObjGrabber:
    def _processObject_(bobj, path, objModCallback: Callable): # Blender object (bobj) to custom object (Obj) linker
        _id = objModCallback("id", bobj, path)
        if (_id == None):
            return None # abort creation

        sceneIds = objModCallback("sceneIds", bobj, path)
        if (sceneIds == None):
            return None # abort creation

        name = objModCallback("name", bobj, path)
        if (name == None):
            return None # abort creation

        return Obj( _id=_id, sceneIds=sceneIds, path=path, _type=bobj.type, name=bobj.name,
                    pos_x=bobj.location.x, pos_y=bobj.location.y, pos_z=bobj.location.z,
                    scale_x=bobj.scale.x, scale_y=bobj.scale.y, scale_z=bobj.scale.z,
                    rot_x=bobj.rotation_euler.x, rot_y=bobj.rotation_euler.y, rot_z=bobj.rotation_euler.z,
                    dim_x=bobj.dimensions.x, dim_y=bobj.dimensions.y, dim_z=bobj.dimensions.z)

    def grabObjects(root_collection, objModCallback: Callable, objs = [], path = ""):
        if (root_collection.name != "Master Collection"):
            path += "/" + root_collection.name
        
        for collection in root_collection.children:
            objs = ObjGrabber.grabObjects(collection, objModCallback, objs, path)
        
        for obj in root_collection.objects:
            ownobj = ObjGrabber._processObject_(obj, path, objModCallback)
            if (ownobj != None):
                objs.append(ownobj)
            
        return objs

# ------------

class FileBuffer:
    def __init__(self, path: str):
        self.path = path
        self.buffer = ""
        return

    def push(self, string:str):
        self.buffer += string + "\n"
        return

    def export(self):
        with open(self.path, "w", encoding="utf-8") as f:
            f.write(self.buffer)
        return

    def __str__(self):
        return self.buffer

# ------------

class NameMap: # name -> id
    def __init__(self, path:str = "", printUnknown: bool = True):
        self.path = path
        self.printUnknown = printUnknown

        self.mapped = {}
        with open(path, "r", encoding="utf-8") as f:
            lines = f.readlines()

        for line in lines:
            if (len(line) > 0 and line[0] != "#"):
                # process given line
                _id = ""
                name = ""
                ignore = False
                idFinished = False
                for char in line:
                    if (char == " "):
                        if (_id != ""):
                            idFinished = True # start loading name
                        else:
                            ignore = True # space encountered before any ids -> wrong syntax -> ignore this line
                            break
                    elif (idFinished == False):
                        _id += char
                    elif (idFinished == True):
                        name += char

                _id = _id.replace("\n", "")
                name = name.replace("\n", "")

                if (_id == "" or name == ""):
                    ignore = True # empty line? ignore

                if (not ignore): # save name -> _id
                    self.mapped[name] = _id
        return

    def _unknownId_(self, name: str):
        if (self.printUnknown == True):
            print("[" + self.path + "] missing: " + name)
        return "X"

    def getId(self, name:str = "") -> str:
        return self.mapped.get(name, self._unknownId_(name)) # return X as default

class NameMapUtils:
    def cleanName(uncleanName: str):
        # modify names to match .obj names (in other words remove .nnn from duplicates and other hints)
        newname = ""
        arraymod = False
        for i, char in enumerate(uncleanName):
            if (char == "#"):
                arraymod = True # arraymod numbers started
            elif (arraymod and char == ","):
                newname += "." # edge case: .nnn dot is flagged as special so comma is used in blender, but makes no sense for 2 different float delimiters to be used so rename it to a dot
                continue
            elif (char == "."):
                arraymod = False # edge case: arraymod numbers ended and blender .nnn started
                continue
            elif (not arraymod and char in ["0","1","2","3","4","5","6","7","8","9"]):
                continue # remove numbers unless they're arraymod parameters
            newname += char
        return newname

class SceneMapUtils: # /s1_s2_s3/abcd -> [1,2,3]
    def extractSceneIds(source: str) -> list:
        scenes = []

        curr = ""
        for i, char in enumerate(source):
            if (i == 0 and char == "/"):
                continue # ignore first / if found
            elif (char in ["/", " ", "_"]):
                if (curr != ""): # save built scene id
                    scenes.append(curr)
                curr = "" # next scene id coming
                loading = False
                if (char == "/"):
                    break # end of scene declarations
            elif (char == "s" and curr == ""):
                loading = True # next scene id coming guaranteed
            elif (char in ["0","1","2","3","4","5","6","7","8","9"] and loading):
                curr += char # building scene id
            else:
                break # unrecognizable format from this point - stop and export whatever exists

        if (len(scenes) == 0):
            scenes.append("X") # no scene numbers found - placeholder X

        return scenes

class SceneMap:
    def __init__(self):
        self.scenes = []

    def appendBySceneId(self, sceneId: str, obj: Obj):
        found = -1
        for i, scene in enumerate(self.scenes):
            if scene[0] == sceneId:
                found = i
                scene[1].append(obj)
                break

        if (found == -1):
            self.scenes.append([sceneId, [obj]])
        
        return
        
    def sort(self):
        # sort the scenes ( [[sceneId, [objs...]]...] ) by their sceneId
        self.scenes = sorted(self.scenes, key = lambda scene: scene[0])
        scenerange = range(len(self.scenes))
        for i in scenerange: # sort the objects under a scene ( [objs...] ) by their id (Obj class assumed)
            self.scenes[i][1] = sorted(self.scenes[i][1], key = lambda obj: obj.sortableId)
        return self.scenes

# -------------

# input
nmap = NameMap("C:/zSchool/PPGSO/Projekt/repo/blender/map_input.txt", printUnknown = False)
# prepare buffer for a savefile
output = FileBuffer("C:/zSchool/PPGSO/Projekt/repo/blender/map.txt")

output.push("# CastleCall Object Map (by iairu)")
output.push("# generated using bpy library and a script for custom syntax definition")
output.push("# ---------------------------------------------------")
output.push("# @scene (@X => unknown)")
output.push("# id pos_x y z rot_x y z scale_x y z dim_x y z")

# output sorting into scene groups
smap = SceneMap()

# modded objects modifier: these attributes are modified from their blender values:
def modder(attr, bobj, path): # mod object attributes on creation, None return => don't create the given object
    if (attr == "id"):
        if not "/triangulated" in path:
            return None # don't create objects outside "/triangulated"
        return nmap.getId(NameMapUtils.cleanName(bobj.name)) # create id from cleaned blender object name
    elif (attr == "sceneIds"):
        path = path.split("/triangulated")[1]
        return SceneMapUtils.extractSceneIds(path) # get scene ids from blender object name
    elif (attr == "name"):
        return NameMapUtils.cleanName(bobj.name) # keep cleaned blender object name

# get the modded objects
objs = ObjGrabber.grabObjects(bpy.context.scene.collection, modder)
for o in objs:
    # only add "triangulated" collection of objects (and don't mention it)
    if isinstance(o, Obj):

        # normalize values to <-1;1>
        # no objects in blender can go beyond max_value, alternatively set to 1 for no normalization
        # currently max object is "the water plane" around 41 in scale, so 100 gives more than 4x reserved space
        # precision in blender has been tested with miniscule movements of objects at around "player scale" and deemeed unimportant under .4f, but just in case it is set higher to .6f (max. shown by blender GUI even though on background it saves .17f)
        # dim appears to be scale normalized to meters (which means either it or scale may be completely useless)
        # some objects (ground_wild) have negative scale because they've been rescaled into inverted position instead of rotation, let's see what happens
        # rotation seems to be in radians so there is no need for normalization as of right now
        o.normalize(max_pos=100, max_scale=100, max_dim=100, max_rot=1, precision=6) 

        # for objects with a different type than mesh/light, which will be handled differently
        # comment them out using "#" (possible because id will be first in line) and add their full names
        # these objects also shouldn't really have an id as of right now but include it just in case
        if (o.type not in ["MESH", "LIGHT"]):
            o.id = "# " + o.id + "_[" + o.name + "]"

        # objects that weren't found in the map_input.txt, comment them out as well, and include their name for debugging
        if (o.id == "X"):
            o.id = "# " + o.id + "_[" + o.name + "]"

        # append the object to every scene where it appears
        for sceneId in o.sceneIds:
            smap.appendBySceneId(sceneId, o)

# sort the scenes
scenes = smap.sort()

# output modified values into buffer which will be exported into a file
for scene in scenes:
    sceneId = scene[0]
    sceneObjs = scene[1]

    output.push("\n@" + sceneId)
    for obj in sceneObjs:
        output.push(str(obj))

# show what will get saved
# print(output)

# save to file
output.export()

# done
print("Done.")
