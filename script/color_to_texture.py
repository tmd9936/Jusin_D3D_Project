import bpy
from pathlib import Path
    

context = bpy.context
scene =  context.scene

colls = bpy.data.collections

Armatures = bpy.data.armatures
#for armature in Armatures:
#    print(dir(armature))


def delete_actions():
    actions = bpy.data.actions

    for actionname in actions.keys():
        if (actionname.startswith('Armature|') == False):
            if ('Mmotion' in actionname):
                actions.remove(actions[actionname])
                

                
def delete_actions2(index):
    actions = bpy.data.actions

    for actionname in actions.keys():
        if (actionname.startswith('Armature|Mmotion') == False):
            if ('M' + str(index) + '_' not in actionname):
                actions.remove(actions[actionname])
                continue
            
def delete_actions_all():
    actions = bpy.data.actions

    for actionname in actions.keys():
        actions.remove(actions[actionname])

def change_bsdf():
    material = bpy.data.materials[0]
    
    material_output = material.node_tree.nodes.get('Material Output')
    material.node_tree.nodes.remove(material.node_tree.nodes.get('Principled BSDF'))
    bsdf = material.node_tree.nodes.new('ShaderNodeBsdfDiffuse')
    normal_map = material.node_tree.nodes.get('Normal Map')
    
    input = bsdf.inputs['Normal']
    output = normal_map.outputs['Normal']
    material.node_tree.links.new(input, output)
    
    input = bsdf.inputs[0]
    output = material_output.outputs['Surface']
    material.node_tree.links.new(input, output)
    

def create_node_color_attribute(obj):
    mat = bpy.data.materials[0]
    
    change_bsdf()
   # obj.data.materials.append(mat)
    #mat.use_nodes = True
    mat.node_tree.nodes.new(type="ShaderNodeVertexColor")
    mat.node_tree.nodes["Color Attribute"].layer_name = "Col"
    
    #mat.node_tree.nodes["Principled BSDF"].is_property_readonly(False)
    #mat.node_tree.nodes["Principled BSDF"].type_recast() = 'ShaderNodeBsdfDiffuse'
    
    input = mat.node_tree.nodes["Diffuse BSDF"].inputs["Base Color"]
    output = mat.node_tree.nodes["Color Attribute"].outputs["Color"]
    mat.node_tree.links.new(input, output)
    
    
def create_node_image_texture(obj, name):
    mat = bpy.data.materials[0]
    
   # obj.data.materials.append(mat)
    #mat.use_nodes = True
    mat.node_tree.nodes.new(type="ShaderNodeTexImage")
    image_src = bpy.data.images.new(name + '.png', 1024, 1024)
    mat.node_tree.nodes["Image Texture"].image = image_src

#    input = mat.node_tree.nodes["Principled BSDF"].inputs["Base Color"]
#    output = mat.node_tree.nodes["Color Attribute"].outputs["Color"]
#    mat.node_tree.links.new(input, output)
    
     
def uv_smart_project(name):
    
    # Get all objects in selection
    obj = bpy.context.selected_objects[1]

    obj.select_set(True)
    # Make it active
    bpy.context.view_layer.objects.active = obj
    # Toggle into Edit Mode
    bpy.ops.object.mode_set(mode='EDIT')
    # Select the geometry
    bpy.ops.mesh.select_all(action='SELECT')
    # Call the smart project operator
    bpy.ops.uv.smart_project()
    # Toggle out of Edit Mode
    create_node_color_attribute(obj)
    create_node_image_texture(obj, name)
    bpy.ops.object.mode_set(mode='OBJECT')
    # Deselect the object
    obj.select_set(False)


def import_fbx():
    for i in range(1,2):
        obj = bpy.ops.import_scene.fbx(filepath='D:/jusin/API/super/po/6/out/' 
            + 'PM' + str(i) + '.fbx')
        target_coll = bpy.data.collections.new(name='PM' + str(i))
        uv_smart_project('PM' + str(i))
        bpy.ops.object.select_all(action='SELECT')
        target_coll.objects.link(bpy.context.selected_objects[1])
        target_coll.objects.link(bpy.context.selected_objects[0])
        scene.collection.objects.unlink(bpy.context.selected_objects[1])
        scene.collection.objects.unlink(bpy.context.selected_objects[0])
        scene.collection.children.link(target_coll)
        #export_all()
        delete_actions_all()
        #remove_all('PM' + str(i))

#https://blender.stackexchange.com/questions/15198/delete-animation-of-object

#for coll in scene.collection.children: # all 
def export_all():
    for coll in scene.collection.children:
        bpy.ops.export_scene.fbx(
            {
                "object" : coll,
                "active_object" : coll,
                "selected_objects" : coll.all_objects,
            },
            filepath=str(
                    "D:/jusin/API/super/po/6/out/" +
                     f"{coll.name}.fbx"
                ),
            use_selection=True,
            )


def remove_all(colName):
    collection = bpy.data.collections.get(colName)

    for obj in collection.objects:
        bpy.data.objects.remove(obj, do_unlink=True)
        
    bpy.data.collections.remove(collection)

def excute():
    import_fbx()
    #export_all()
    #delete_actions_all()
    #remove_all()

excute()
#import_fbx()            
#export_all()         