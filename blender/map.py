import bpy
import os

# HOW TO RUN: open in blender using built-in "Text Editor" or built-in "Python Console"
# /\ will resolve bpy and auto-import currently opened blender project as bpy.context

# DON'T OPEN IN NORMAL TERMINAL, or you will get errors because of missing context

class Obj:
    def __init__(self,  name:str = "?", path:str = "", type:str = "LIGHT",
                        pos_x:float = 0.0, pos_y:float = 0.0, pos_z:float = 0.0,
                        rot_x:float = 0.0, rot_y:float = 0.0, rot_z:float = 0.0,
                        scale_x:float = 0.0, scale_y:float = 0.0, scale_z:float = 0.0,
                        dim_x:float = 0.0, dim_y:float = 0.0, dim_z:float = 0.0,
                        uv_file:str = ""):
        self.name = name
        self.path = path
        self.type = type
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

        self.uv_file = uv_file
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
        return  self.path + " " + self.type + " " + self.name + " " + \
                format(self.pos_x, "." + str(self.precision) + "f") + " " + format(self.pos_y, "." + str(self.precision) + "f") + " " + format(self.pos_z, "." + str(self.precision) + "f") + " " + \
                format(self.rot_x, "." + str(self.precision) + "f") + " " + format(self.rot_y, "." + str(self.precision) + "f") + " " + format(self.rot_z, "." + str(self.precision) + "f") + " " + \
                format(self.scale_x, "." + str(self.precision) + "f") + " " + format(self.scale_y, "." + str(self.precision) + "f") + " " + format(self.scale_z, "." + str(self.precision) + "f") + " " + \
                format(self.dim_x, "." + str(self.precision) + "f") + " " + format(self.dim_y, "." + str(self.precision) + "f") + " " + format(self.dim_z, "." + str(self.precision) + "f") + " " + \
                self.uv_file
    
# ------------

class Utils:
    def _fixSlashes_(string: str, to: str, removeDuplicates: bool):
        out = ""
        pchar = ""
        for char in string:
            if (char in ["/","\\"]):
                if (removeDuplicates and pchar == char):
                    continue
                out += to
            else:
                out += char
            pchar = char

        return out
    
    def _getMaterialTextureFilepath_(bobj): # gets filepath of the image texture of the first material, else empty string
        if (not bobj.type == "MESH" or len(bobj.data.materials) < 1):
            return ""

        material = bobj.data.materials[0]

        path = ""
        for node in material.node_tree.nodes:
            if not (node.name == "Image Texture"):
                continue
            path = node.image.filepath
        return Utils._fixSlashes_(path, "/", True)
        

    def _processObject_(bobj, path): # Blender object (bobj) to custom object (Obj) linker
        return Obj( name=bobj.name, path=path, type=bobj.type,
                    pos_x=bobj.location.x, pos_y=bobj.location.y, pos_z=bobj.location.z,
                    scale_x=bobj.scale.x, scale_y=bobj.scale.y, scale_z=bobj.scale.z,
                    rot_x=bobj.rotation_euler.x, rot_y=bobj.rotation_euler.y, rot_z=bobj.rotation_euler.z,
                    dim_x=bobj.dimensions.x, dim_y=bobj.dimensions.y, dim_z=bobj.dimensions.z,
                    uv_file=Utils._getMaterialTextureFilepath_(bobj))

    def grabObjects(root_collection, objs = [], path = ""):
        if (root_collection.name != "Master Collection"):
            path += "/" + root_collection.name
        
        for collection in root_collection.children:
            objs = Utils.grabObjects(collection, objs, path)
        
        for obj in root_collection.objects:
            objs.append(Utils._processObject_(obj, path))
            
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

# prepare buffer for a savefile
output = FileBuffer("C:/zSchool/PPGSO/Projekt/blender/map.txt")

output.push("# CastleCall Object Map (by iairu)")
output.push("# generated using bpy library and a script for custom syntax definition")
output.push("# ---------------------------------------------------")
output.push("# name: char == #: arraymod is present #x;y;z;n (x;y;z clone translation RELATIVE to object space, n is clone count)")
output.push("# path type name pos_x y z rot_x y z scale_x y z dim_x y z (uv_file if type == MESH)")

objs = Utils.grabObjects(bpy.context.scene.collection)
for o in objs:
    # only add "triangulated" collection of objects (and don't mention it)
    if isinstance(o, Obj) and "/triangulated" in o.path:
        o.path = o.path.split("/triangulated")[1]

        # remove .nnn from equivalently named "clones" collection
        # must be last collection in given part of hierarchy to work well
        if "/clones" in o.path:
            o.path = o.path.split("/clones")[0] + "/clones"

        # modify names to match .obj names (in other words remove .nnn from duplicates and other hints)
        newname = ""
        arraymod = False
        for i, char in enumerate(o.name):
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
        o.name = newname

        # normalize values to <-1;1>
        # no objects in blender can go beyond max_value, alternatively set to 1 for no normalization
        # currently max object is "the water plane" around 41 in scale, so 100 gives more than 4x reserved space
        # precision in blender has been tested with miniscule movements of objects at around "player scale" and deemeed unimportant under .4f, but just in case it is set higher to .6f (max. shown by blender GUI even though on background it saves .17f)
        # dim appears to be scale normalized to meters (which means either it or scale may be completely useless)
        # some objects (ground_wild) have negative scale because they've been rescaled into inverted position instead of rotation, let's see what happens
        # rotation seems to be in radians so there is no need for normalization as of right now
        o.normalize(max_pos=100, max_scale=100, max_dim=100, max_rot=1, precision=6) 

        # output modified values into buffer which will be exported into a file
        output.push(str(o))

# show what will get saved
print(output)

# save to file
output.export()
