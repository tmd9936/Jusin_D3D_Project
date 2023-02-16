import bpy
from pathlib import Path
    

context = bpy.context
scene =  context.scene

colls = bpy.data.collections

Armatures = bpy.data.armatures
for armature in Armatures:
    print(dir(armature))


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
                
            

def import_fbx():
    for i in range(1,4):
        obj = bpy.ops.import_scene.fbx(filepath='D:/jusin/API/super/po/6/GameObject/GameObject/' 
            + 'PM' + str(i) + '.fbx')
        target_coll = bpy.data.collections.new(name='PM' + str(i))
        target_coll.objects.link(bpy.context.selected_objects[1])
        target_coll.objects.link(bpy.context.selected_objects[0])
        scene.collection.objects.unlink(bpy.context.selected_objects[1])
        scene.collection.objects.unlink(bpy.context.selected_objects[0])
        scene.collection.children.link(target_coll)
        delete_actions2(str(i))
        export_all()
        delete_actions_all()
        remove_all('PM' + str(i))

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