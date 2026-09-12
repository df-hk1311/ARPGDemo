"""Create the placeholder levels used by the ARPG flow foundation."""

import unreal


MENU_LEVEL = "/Game/ARPG/Flow/L_MainMenu"
ARENA_LEVEL = "/Game/ARPG/Flow/L_Arena"


def log(message):
    unreal.log(f"[ARPG SetupFlowMaps] {message}")


def ensure_level(path):
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        log(f"Level already exists: {path}")
        return False

    level_editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if not level_editor.new_level(path):
        raise RuntimeError(f"Failed to create level: {path}")

    log(f"Created level: {path}")
    return True


def save_current_level():
    level_editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if not level_editor.save_current_level():
        raise RuntimeError("Failed to save the current level")


def spawn_actor(actor_class, location, rotation=None):
    actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    return actor_subsystem.spawn_actor_from_class(
        actor_class,
        location,
        rotation if rotation is not None else unreal.Rotator(0.0, 0.0, 0.0),
    )


def add_arena_geometry():
    cube = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cube.Cube")
    if not cube:
        raise RuntimeError("Could not load /Engine/BasicShapes/Cube.Cube")

    floor = spawn_actor(unreal.StaticMeshActor, unreal.Vector(0.0, 0.0, -110.0))
    floor.set_actor_label("ArenaFloor")
    floor.set_actor_scale3d(unreal.Vector(20.0, 20.0, 0.2))
    floor.static_mesh_component.set_static_mesh(cube)

    player_start = spawn_actor(unreal.PlayerStart, unreal.Vector(0.0, 0.0, 100.0))
    player_start.set_actor_label("PlayerStart")

    directional_light = spawn_actor(
        unreal.DirectionalLight,
        unreal.Vector(0.0, 0.0, 500.0),
        unreal.Rotator(-45.0, 0.0, 0.0),
    )
    directional_light.set_actor_label("DirectionalLight")

    sky_light = spawn_actor(unreal.SkyLight, unreal.Vector(0.0, 0.0, 500.0))
    sky_light.set_actor_label("SkyLight")


def main():
    if ensure_level(MENU_LEVEL):
        save_current_level()

    if ensure_level(ARENA_LEVEL):
        add_arena_geometry()
        save_current_level()

    log("Flow maps are ready.")


if __name__ == "__main__":
    main()