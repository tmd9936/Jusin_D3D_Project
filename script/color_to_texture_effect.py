import bpy
import os
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
            
def delete_actions3(name):
    actions = bpy.data.actions

    for actionname in actions.keys():
        if (name not in actionname):
            actions.remove(actions[actionname])    

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
    
    output = bsdf.outputs['BSDF']
    input = material_output.inputs['Surface']
    material.node_tree.links.new(input, output)
    
    bpy.context.object.active_material = material
    
    
def change_bsdf2():
    material = bpy.data.materials[0]
    
    material_output = material.node_tree.nodes.get('Material Output')
    material.node_tree.nodes.remove(material.node_tree.nodes.get('Diffuse BSDF'))
    bsdf = material.node_tree.nodes.new('ShaderNodeBsdfPrincipled')
    normal_map = material.node_tree.nodes.get('Normal Map')
    
    input = bsdf.inputs['Normal']
    output = normal_map.outputs['Normal']
    material.node_tree.links.new(input, output)
    
    output = bsdf.outputs['BSDF']
    input = material_output.inputs['Surface']
    material.node_tree.links.new(input, output)
    
    material.node_tree.nodes.remove(material.node_tree.nodes.get("Color Attribute"))
    
    input = bsdf.inputs["Base Color"]
    output = material.node_tree.nodes["Image Texture"].outputs["Color"]
    material.node_tree.links.new(input, output)
    
    bpy.context.object.active_material = material
    

def create_node_color_attribute():
    mat = bpy.data.materials[0]
    
    change_bsdf()
   # obj.data.materials.append(mat)
    #mat.use_nodes = True
    mat.node_tree.nodes.new(type="ShaderNodeVertexColor")
    mat.node_tree.nodes["Color Attribute"].layer_name = "Col"
    
    #mat.node_tree.nodes["Principled BSDF"].is_property_readonly(False)
    #mat.node_tree.nodes["Principled BSDF"].type_recast() = 'ShaderNodeBsdfDiffuse'
    
    mat.node_tree.nodes["Diffuse BSDF"].inputs["Roughness"].default_value = 0.533
    
    input = mat.node_tree.nodes["Diffuse BSDF"].inputs["Color"]
    output = mat.node_tree.nodes["Color Attribute"].outputs["Color"]
    mat.node_tree.links.new(input, output)
    
    bpy.context.object.active_material = mat
    mat.use_nodes = True
    
    
def create_node_image_texture(name):
    mat = bpy.data.materials[0]
    
   # obj.data.materials.append(mat)
    mat.use_nodes = True
    nodes = mat.node_tree.nodes
    texture_node = mat.node_tree.nodes.new(type="ShaderNodeTexImage")
    image_src = bpy.data.images.new(name + '.png', 1024, 1024)
    texture_node.select = True
    nodes.active = texture_node
    texture_node.image = image_src
#    input = mat.node_tree.nodes["Principled BSDF"].inputs["Base Color"]
#    output = mat.node_tree.nodes["Color Attribute"].outputs["Color"]
#    mat.node_tree.links.new(input, output)


def bake():
    bpy.context.scene.render.engine = 'CYCLES'
    bpy.context.scene.cycles.device = 'GPU'
    bpy.context.scene.cycles.bake_type = 'DIFFUSE'
    bpy.context.scene.render.bake.use_pass_direct = False
    bpy.context.scene.render.bake.use_pass_indirect = False
    bpy.context.scene.render.bake.margin = 0
    bpy.context.scene.render.bake.target = 'IMAGE_TEXTURES'
    
    # Bake selected objects
    count = 0
    for obj in filter(lambda x: x.type == 'MESH', bpy.data.objects):
        print('Selected: ' + obj.name)
        
        # Select current object
        bpy.ops.object.select_all(action='DESELECT')
        bpy.context.view_layer.objects.active = obj
        obj.select_set(True)
        bpy.ops.object.bake(type='DIFFUSE', pass_filter={'COLOR'}, use_clear=False)


def texture_properties_setting(name):
    texture = bpy.ops.texture.new()
    texture = bpy.data.textures["Texture"]
    texture.type = "IMAGE" #changing type
    texture = bpy.data.textures["Texture"] # This one has noise_scale attribute
    texture.image = bpy.data.images[name + '.png']  #changing noise scale from this new type 


def texture_save(name):
    texture = bpy.data.textures["Texture"]    
    texture.image.save_render(filepath="D:/jusin/API/super/po/6/out4/" + name + '.png')
    texture.image.source = 'FILE'
    texture.image.filepath = "D:/jusin/API/super/po/6/out4/" + name + '.png'
    
     
def uv_smart_project(obj, name):
    
    # Get all objects in selection

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
    #create_node_color_attribute()
    #create_node_image_texture(name)
    #bpy.ops.object.mode_set(mode='OBJECT')
    #texture_properties_setting(name)
    #bake()
    #change_bsdf2()
    #texture_save(name)
    # Deselect the object
    #obj.select_set(False)


def import_fbx():
    eName = 'E_BBB_Denki'
    obj = bpy.ops.import_scene.fbx(filepath='D:/jusin/API/super/po/9/Effect/' + eName + '.fbx')
    target_coll = bpy.data.collections.new(name=eName)
    uv_smart_project(eName)
    bpy.ops.object.select_all(action='SELECT')
    for object in bpy.context.selected_objects:
        target_coll.objects.link(object)
    for object in bpy.context.selected_objects:
        scene.collection.objects.unlink(object)
    scene.collection.children.link(target_coll)
    delete_actions3()
    #export_all(eName)
    #delete_actions_all()
    #remove_all()

#https://blender.stackexchange.com/questions/15198/delete-animation-of-object

#for coll in scene.collection.children: # all 
def export_all(name):
    for coll in scene.collection.children:
        if coll.name == name:
            bpy.ops.export_scene.fbx(
                {
                    "object" : coll,
                    "active_object" : coll,
                    "selected_objects" : coll.all_objects,
                },
                filepath=str(
                        "D:/jusin/API/super/po/6/out4/" +
                         f"{coll.name}.fbx"
                    ),
                use_selection=True,
                )


def remove_all():     
    # Remove all actions
    for action in bpy.data.actions:
        bpy.data.actions.remove(action)

    # Remove all armatures
    for armature in bpy.data.armatures:
        bpy.data.armatures.remove(armature)

    # Remove all collections
    for collection in bpy.data.collections:
        bpy.data.collections.remove(collection)

    # Remove all images
    for image in bpy.data.images:
        bpy.data.images.remove(image)

    # Remove all materials
    for material in bpy.data.materials:
        bpy.data.materials.remove(material)

    # Remove all meshes
    for mesh in bpy.data.meshes:
        bpy.data.meshes.remove(mesh)

    # Remove all objects
    for obj in bpy.data.objects:
        bpy.data.objects.remove(obj)

    # Remove all textures
    for texture in bpy.data.textures:
        bpy.data.textures.remove(texture)
        
        # Remove all textures
    for particle in bpy.data.particles:
        bpy.data.particles.remove(particle)
        

def excute():
    import_fbx()
    #export_all()
    #delete_actions_all()
    #remove_all()

remove_all()
color_pixel_index = 0
pixels = [0.5] * (32 * 32 * 4)
def set_color(objects):
    global color_pixel_index
    global pixels
    if (color_pixel_index >= (32 * 32 * 4)):
        return
    if (objects.type == 'MESH'):        
        color = objects.color
        pixels[color_pixel_index * 4 + 0] = int(color[0] * 255)
        pixels[color_pixel_index * 4 + 1] = int(color[1] * 255)
        pixels[color_pixel_index * 4 + 2] = int(color[2] * 255)
        pixels[color_pixel_index * 4 + 3] = 255
        color_pixel_index += 1
    if (len(objects.children) <= 0):
        return
    for obj in objects.children:
        set_color(obj)
        

def particle_export():
    # Load the FBX file with particle information and color attribute information
    bpy.ops.import_scene.fbx(filepath='D:/jusin/API/super/po/9/Effect/' + 'E_BBB_Denki' + '.fbx')

    delete_actions3('E_BBB_Denki')

    # Get the object that works like particles
    #objects = bpy.data.objects['E_BBB_Denki']
    
    objects = bpy.data.objects['E_BBB_Denki']

    objects.select_set(True)
    # Make it active
    bpy.context.view_layer.objects.active = objects

    mat = bpy.data.materials[0]
    bpy.context.object.active_material = mat
    mat.use_nodes = True

    # Create a new image and paint the color attribute information into it
    img = bpy.data.images.new("object_color", width=32, height=32)
    #pixels = [0] * (512 * 512 * 4)
    global pixels
    set_color(objects)
    img.pixels = pixels

    # Create a new texture and assign the image to it
    tex = bpy.data.textures.new("object_color", type='IMAGE')
    tex.image = img

    # Assign the texture to the material
    mat.use_nodes = True
    nodes = mat.node_tree.nodes
    tex_node = nodes.new("ShaderNodeTexImage")
    tex_node.image = img

    # Connect the texture to the material output
    #output_node = nodes.get("Material Output")
    #links = mat.node_tree.links
    #link = links.new(tex_node.outputs[0], output_node.inputs[0])

    # Assign the material to the object
    #if len(objects.data.materials) == 0:
    #    objects.data.materials.append(mat)

    # Save the FBX file with particle information and texture information
    #bpy.ops.export_scene.fbx(filepath="D:/jusin/API/super/po/6/out4/" + 'E_BBB_Denki' + '.fbx')

def set_particle(objects, particle_systems):
    if (objects.type == 'MESH'):        
        objects.select_set(True)

        # Convert the object into a particle system
        particle_systems.append(objects)
    if (len(objects.children) <= 0):
        return
    for obj in objects.children:
        set_particle(obj, particle_systems)


def make_particle(name):
    bpy.ops.import_scene.fbx(filepath='D:/jusin/API/super/po/9/Effect/' + name + '.fbx')

    delete_actions3(name)    
    
    
    mat = None
    if (len(bpy.data.materials) <= 0):
        mat = bpy.data.materials.new(name + "_bake_material")
    else:
        mat = bpy.data.materials[0]
    
    action_length = 1
    # Connect the objects to animations
    for obj in bpy.data.objects:
        if (obj.type == 'MESH'):
            for anim in bpy.data.actions:
                if (obj.name + '|' in anim.name) and (anim.name.startswith(obj.name)):
                    obj.data.materials.append(mat)
                    print(obj.name, anim.name)
                    obj.animation_data_create()
                    obj.animation_data.action = anim
                    
                    obj.animation_data.use_nla = False                  
                    
                    action = obj.animation_data.action
                    
                    # Get the length of the action
                    action_length_temp = action.frame_range[1] - action.frame_range[0]
                    
                    uv_smart_project(obj, obj.name)
                    
                    if (action_length < action_length_temp):
                        action_length = action_length_temp
                        # Set the scene's frame range to match the action's length
                        bpy.context.scene.frame_start = 0
                        bpy.context.scene.frame_end = int(action_length) + 5
                    
                    break
     
     
    bpy.ops.object.select_all(action='SELECT')
    
    
    objects = bpy.data.objects[name]

    objects.select_set(True)
    # Make it active
    bpy.context.view_layer.objects.active = objects
    
    bpy.context.object.active_material = mat
    mat.use_nodes = True        
                
    create_node_color_attribute()
    create_node_image_texture(name)
    bpy.ops.object.mode_set(mode='OBJECT')
    texture_properties_setting(name)
    bake()
    change_bsdf2()
    texture_save(name)

#    objects = bpy.data.objects['E_BBB_Denki']

#    objects.select_set(True)
#    # Make it active
#    bpy.context.view_layer.objects.active = objects

#    mat = bpy.data.materials[0]
#    bpy.context.object.active_material = mat
#    mat.use_nodes = True
#    
#    bpy.ops.object.mode_set(mode='EDIT')
#    # Select the geometry
#    bpy.ops.mesh.select_all(action='SELECT')
#    # Call the smart project operator
#    bpy.ops.uv.smart_project()
   

    # Export the scene as an FBX file
    
    bpy.ops.export_scene.fbx(filepath="D:/jusin/API/super/po/6/out4/" + name + '.fbx',
    use_selection=False, global_scale=1.0, apply_unit_scale=True, 
    bake_space_transform=False, object_types={'EMPTY', 'MESH'}, use_mesh_modifiers=True, 
    use_mesh_modifiers_render=True, mesh_smooth_type='FACE', use_mesh_edges=False,
     use_tspace=False, use_custom_props=False, add_leaf_bones=False, primary_bone_axis='Y', 
     secondary_bone_axis='X', use_armature_deform_only=False, bake_anim=True, 
     bake_anim_use_all_actions=False, bake_anim_use_nla_strips=False, 
     bake_anim_use_all_bones=True, bake_anim_force_startend_keying=True, bake_anim_step=1.0, bake_anim_simplify_factor=1.0)

dir_path = 'D:/jusin/API/super/po/9/Effect/'

def export_effect():
    for (root, directories, files) in os.walk(dir_path):
        for file in files:
            if '.fbx' in file:
                name, ext = os.path.splitext(file)
                make_particle(name)
                remove_all()
export_effect()
#excute()
#import_fbx()            
#export_all()     
#particle_export()    