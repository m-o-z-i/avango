#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import time
import random

import examples_common.navigator
from examples_common.GuaVE import GuaVE
from SlideSwitcher import SlideSwitcher

SLIDE_OFFSET = 5.0

class Animation(avango.script.Script):
  Duration = avango.SFFloat()

  StartPos = avango.gua.SFVec3()
  EndPos   = avango.gua.SFVec3()

  StartRot = avango.gua.SFQuat()
  EndRot   = avango.gua.SFQuat()

  CurrentMatrix = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(Animation).__init__()
    self.always_evaluate(True)
    self._start = int(round(time.time() * 1000))

  def evaluate(self):
    _now = int(round(time.time() * 1000))
    if (_now - self._start < self.Duration.value):
      _pos = self.StartPos.value.lerp_to(self.EndPos.value, (_now-self._start)/self.Duration.value)
      _rot = self.StartRot.value.slerp_to(self.EndRot.value, (_now-self._start)/self.Duration.value)
      self.CurrentMatrix.value = avango.gua.make_trans_mat(_pos) * avango.gua.make_rot_mat(_rot.get_angle(), _rot.get_axis())

class TimedMaterialUniformUpdate(avango.script.Script):

  TimeIn = avango.SFFloat()
  MaterialName = avango.SFString()
  UniformName = avango.SFString()

  @field_has_changed(TimeIn)
  def update(self):
    avango.gua.set_material_uniform(self.MaterialName.value,
                                    self.UniformName.value,
                                    self.TimeIn.value)

# for avango example
class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*30.0, 0.0, 1.0, 0.0)

def setup_pipe():
  camera = avango.gua.nodes.Camera(
                                   #View = "/navigation/view",
                                   LeftEye = "/navigation/view",
                                   RightEye = "/navigation/view",
                                   #Screen = "/navigation/screen",
                                   LeftScreen = "/navigation/screen",
                                   RightScreen = "/navigation/screen",
                                   SceneGraph = "scenegraph")

  #width = 1800
  #height = 1180
  width = 1024
  height = 768
  size = avango.gua.Vec2ui(width, height)

  window = avango.gua.nodes.Window(Size = size,
                                   Title = "vrgeo2013",
                                   LeftResolution = size,
                                   EnableVsync = True
                                   )

  pipe = avango.gua.nodes.Pipeline(Camera = camera,
                                   Window = window,
                                   LeftResolution = size)


  avango.gua.create_texture("data/textures/sky.jpg")
  avango.gua.create_texture("data/textures/sky2.jpg")
  avango.gua.create_texture("data/textures/sky3.jpg")

  pipe.EnableFXAA.value = False
  pipe.EnableFrustumCulling.value = False
  pipe.EnableBackfaceCulling.value = False

  pipe.EnableHDR.value = False
  pipe.HDRKey.value = 3.0

  pipe.EnableFog.value = True
  pipe.FogTexture.connect_from(pipe.BackgroundTexture)
  pipe.FogStart.value = 250.0
  pipe.FogEnd.value = 580.0

  pipe.BackgroundTexture.value = "data/textures/sky.jpg"

  pipe.AmbientColor.value = avango.gua.Color(0.8, 0.8, 0.8)

  pipe.EnableSsao.value = True
  pipe.SsaoIntensity.value = 3.0
  pipe.SsaoRadius.value = 5.0

  pipe.EnableBloom.value = True
  pipe.BloomRadius.value = 10.0
  pipe.BloomThreshold.value = 0.9
  pipe.BloomIntensity.value = 0.3

  return pipe

def reload():
  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph  = avango.gua.nodes.SceneGraph(Name = "scenegraph")
  loader = avango.gua.nodes.GeometryLoader()

  oilrig_cs = avango.gua.create_triangle_mesh_shape_from_geometry_file(
    "oilrig_cs",
    "data/objects/OIL_RIG_GUACAMOLE/oilrig.obj",
    True,
    False,
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )
  oilrig_cs.Scaling.value = avango.gua.Vec3(0.1, 0.1, 0.1)


  oilrig_csn = avango.gua.nodes.CollisionShapeNode(
    Name = "oilrig_csn",
    ShapeName = "oilrig_cs"
  )

  oilrig = loader.create_geometry_from_file("town",
          "data/objects/OIL_RIG_GUACAMOLE/oilrig.obj",
          "White",
          avango.gua.LoaderFlags.LOAD_MATERIALS |
          avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY )
  oilrig.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 0.1)

  oilrig_rb = avango.gua.nodes.RigidBodyNode(
    Name = "oilrig_rb",
    Mass = 0.0,
    Restitution = 10.0,
    Transform = avango.gua.make_rot_mat(-90, 1, 0, 0),
    Children = [oilrig_csn]
  )

  oilrig_csn.Children.value = [oilrig]



  water = loader.create_geometry_from_file("water_geometry",
          "data/objects/plane.obj",
          "Water",
          avango.gua.LoaderFlags.DEFAULTS)

  water.Transform.value = avango.gua.make_scale_mat(1000, 1, 1000) * avango.gua.make_trans_mat(0, -15, 0)

  slides = loader.create_geometry_from_file("slides",
           "data/objects/test_slides.obj",
           "White",
           # avango.gua.LoaderFlags.DEFAULTS)
           avango.gua.LoaderFlags.LOAD_MATERIALS)


  sun = avango.gua.nodes.SpotLightNode(Name = "sun",
                                       Color = avango.gua.Color(1.0, 0.8, 0.6),
                                       Falloff = 0.009,
                                       Softness = 0.003,
                                       EnableDiffuseShading = True,
                                       EnableSpecularShading = False,
                                       EnableShadows = True,
                                       ShadowMapSize = 4096,
                                       ShadowOffset = 0.0005)

  sun.Transform.value = avango.gua.make_trans_mat(-10.0, 20.0, 40.0) * \
                        avango.gua.make_scale_mat(120, 120, 80) * \
                        avango.gua.make_rot_mat(-25.0, 1.0, 0.0, 0.0)

  head_light = avango.gua.nodes.PointLightNode(Name = "head",
                                       Color = avango.gua.Color(1.0, 0.8, 0.6),
                                       Falloff = 1,
                                       EnableDiffuseShading = True,
                                       EnableSpecularShading = False,
                                       Transform = avango.gua.make_scale_mat(5, 5, 5))

  fake_sun = avango.gua.nodes.SpotLightNode(Name = "fake_sun",
                                            Color = avango.gua.Color(1.2, 0.5, 0.3),
                                            Falloff = 1.0,
                                            Softness = 1.0,
                                            EnableShadows = False,
                                            EnableSpecularShading = False,
                                            EnableDiffuseShading = False,
                                            EnableGodrays = True)

  fake_sun.Transform.value = avango.gua.make_trans_mat(-100, 6, 200) * avango.gua.make_scale_mat(100, 100, 100)

  navigation = avango.gua.nodes.TransformNode(Name = "navigation")
  navigation.Transform.value = avango.gua.make_trans_mat(14.501, 7.142, -9.199)*avango.gua.make_rot_mat(155, 0, 1, 0)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       #Width = 3.0,
                                       #Height = 1.97)
                                       Width = 4.0, Height = 3)
  slide_transform = avango.gua.nodes.TransformNode(Name = "slideTransform")
  slide_transform.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0) * avango.gua.make_rot_mat(-90, 0, 1, 0)

  screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.5, 0.0)
  #screen.Transform.value = avango.gua.make_trans_mat(0.0, 1.38, -1.57)

  #view = avango.gua.nodes.ViewNode(Name = "view", StereoWidth = 0.3)
  view = avango.gua.nodes.TransformNode(Name = "view")
  view.Transform.value = avango.gua.make_trans_mat(0.0, 1.5, 2.0)

  ##############################################################################
  ## examples
  ##############################################################################

  timer = avango.nodes.TimeSensor()
  timed_rotate = TimedRotate();
  timed_rotate.TimeIn.connect_from(timer.Time)

  # avango example
  avango_example = avango.gua.nodes.TransformNode(Name = "avango_example")
  #avango_example.Transform.value = avango.gua.make_trans_mat(0, 1, -4.2*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  avango_example.Transform.value = avango.gua.make_trans_mat(0, 1, -17.2*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  monkey = loader.create_geometry_from_file("monkey",
           "data/objects/monkey.obj",
           "CarPaintOrange", avango.gua.LoaderFlags.DEFAULTS)
  avango_example.Children.value = [monkey]

  # different kinds of data
  different_kinds_of_data = avango.gua.nodes.TransformNode(Name = "different_kinds_of_data")
  #different_kinds_of_data.Transform.value = avango.gua.make_trans_mat(-1, 1, -4.0*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.4, 0.4, 0.4)
  different_kinds_of_data.Transform.value = avango.gua.make_trans_mat(-0.3, 1, -4.0*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.4, 0.4, 0.4)

  dk_mesh = loader.create_geometry_from_file("dk_mesh",
           "data/objects/monkey.obj",
           "CarPaintOrange", avango.gua.LoaderFlags.DEFAULTS)
  #dk_mesh.Transform.value = avango.gua.make_scale_mat(1.5,1.5,1.5)

  dk_mesh_rotation = avango.gua.nodes.TransformNode(Name = "dk_mesh_rotation")
  dk_mesh_rotation.Transform.connect_from(timed_rotate.MatrixOut)
  dk_mesh_rotation.Children.value = [dk_mesh]

  dk_mesh_translation = avango.gua.nodes.TransformNode(Name = "dk_mesh_translation")
  dk_mesh_translation.Transform.value = avango.gua.make_trans_mat(0, 0, 3.0)
  dk_mesh_translation.Children.value = [dk_mesh_rotation]

  dk_nurbs = loader.create_geometry_from_file("dk_nurbs",
           #"data/objects/monkey.obj",
           #"data/objects/heckscheibe.igs",
           "data/objects/rim.igs",
           "CarPaintOrange", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.NORMALIZE_POSITION | avango.gua.LoaderFlags.NORMALIZE_SCALE)
  dk_nurbs.Transform.value = avango.gua.make_scale_mat(3,3,3) * dk_nurbs.Transform.value

  dk_nurbs_rotation = avango.gua.nodes.TransformNode(Name = "dk_nurbs_rotation")
  dk_nurbs_rotation.Transform.connect_from(timed_rotate.MatrixOut)
  dk_nurbs_rotation.Children.value = [dk_nurbs]

  dk_nurbs_translate = avango.gua.nodes.TransformNode(Name = "dk_nurbs_translate")
  #dk_nurbs_translate.Transform.value = avango.gua.make_scale_mat(0.005, 0.005, 0.005)
  dk_nurbs_translate.Transform.value = avango.gua.make_scale_mat(1.0, 1.0, 1.0)
  dk_nurbs_translate.Children.value = [dk_nurbs_rotation]

  dk_vol = loader.create_geometry_from_file("dk_vol",
           "data/objects/head_w256_h256_d225_c1_b8.raw",
           # "data/objects/monkey.obj",
           "CarPaintOrange", avango.gua.LoaderFlags.DEFAULTS | avango.gua.LoaderFlags.NORMALIZE_POSITION | avango.gua.LoaderFlags.NORMALIZE_SCALE)

  dk_vol.Transform.value = avango.gua.make_scale_mat(2,2,2) * avango.gua.make_rot_mat(90.0, avango.gua.Vec3(1,0,0)) * dk_vol.Transform.value

  dk_vol_rotation = avango.gua.nodes.TransformNode(Name = "dk_vol_rotation")
  dk_vol_rotation.Transform.connect_from(timed_rotate.MatrixOut)
  dk_vol_rotation.Children.value = [dk_vol]

  dk_vol_translation = avango.gua.nodes.TransformNode(Name = "dk_vol_translation")
  dk_vol_translation.Transform.value = avango.gua.make_trans_mat(0, 0, -3.0)
  dk_vol_translation.Children.value = [dk_vol_rotation]

  different_kinds_of_data.Children.value = [dk_mesh_translation,dk_nurbs_translate,dk_vol_translation]

  # python example
  def make_noon():
    pipe.BackgroundTexture.value = "data/textures/sky2.jpg"
    pipe.AmbientColor.value = avango.gua.Color(0.8, 0.8, 1.0)
    sun.Color.value = avango.gua.Color(2.0, 2.0, 2.5)
    sun.Transform.value = sun.Transform.value = avango.gua.make_trans_mat(-10.0, 40.0, 40.0) * avango.gua.make_scale_mat(120, 120, 80) * avango.gua.make_rot_mat(-45.0, 1.0, 0.0, 0.0)
    fake_sun.Color.value = avango.gua.Color(0, 0, 0)
    avango.gua.set_material_uniform("Water", "skymap", pipe.BackgroundTexture.value)

  def make_evening():
    pipe.BackgroundTexture.value = "data/textures/sky.jpg"
    pipe.AmbientColor.value = avango.gua.Color(0.6, 0.6, 1.0)
    sun.Color.value = avango.gua.Color(1.0, 0.8, 0.6)
    sun.Transform.value = avango.gua.make_trans_mat(-10.0, 20.0, 40.0) * \
                          avango.gua.make_scale_mat(120, 120, 120) * \
                          avango.gua.make_rot_mat(-25.0, 1.0, 0.0, 0.0)
    fake_sun.Color.value = avango.gua.Color(1.2, 0.5, 0.3)
    avango.gua.set_material_uniform("Water", "skymap", pipe.BackgroundTexture.value)

  def make_night():
    pipe.BackgroundTexture.value = "data/textures/sky3.jpg"
    pipe.AmbientColor.value = avango.gua.Color(0.0, 0.0, 0.0)
    sun.Color.value = avango.gua.Color(1.0, 1.0, 3.5)
    sun.Transform.value = avango.gua.make_trans_mat(-10.0, 20.0, 40.0) * avango.gua.make_scale_mat(120, 120, 80) *avango.gua.make_rot_mat(-25.0, 1.0, 0.0, 0.0)
    fake_sun.Color.value = avango.gua.Color(0, 0, 0)
    avango.gua.set_material_uniform("Water", "skymap", pipe.BackgroundTexture.value)

  # material example
  material_example = avango.gua.nodes.TransformNode(Name = "material_example")
  #material_example.Transform.value = avango.gua.make_trans_mat(0, 1, -15*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  material_example.Transform.value = avango.gua.make_trans_mat(-0.4, 1, -5*SLIDE_OFFSET) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)

  material_example_group = avango.gua.nodes.TransformNode(Name = "material_example_group")
  #material_example_group.Transform.connect_from(timed_rotate.MatrixOut)

  material_example.Children.value = [material_example_group]

  def _display_material(parent, row, column, name):
    column_width = 2.0
    row_width = 2.0

    sphere = loader.create_geometry_from_file(
      name, "data/objects/sphere.obj", name,
      avango.gua.LoaderFlags.DEFAULTS
    )

    plane = loader.create_geometry_from_file(
      name, "data/objects/plane.obj", name,
      avango.gua.LoaderFlags.DEFAULTS
    )

    monkey = loader.create_geometry_from_file(
      name, "data/objects/monkey.obj", name,
      avango.gua.LoaderFlags.DEFAULTS
    )

    sphere.Transform.value = avango.gua.make_trans_mat(row*row_width, 0.0, column*column_width) * \
                             avango.gua.make_scale_mat(0.4, 0.4, 0.4)
    plane.Transform.value =  avango.gua.make_trans_mat(row*row_width, -0.4, column*column_width) * \
                             avango.gua.make_scale_mat(2, 1, 2)
    monkey.Transform.value = avango.gua.make_trans_mat(row*row_width, 0.8, column*column_width) * \
                             avango.gua.make_rot_mat(90.0, 0.0, 1.0, 0.0) * \
                             avango.gua.make_scale_mat(0.3, 0.3, 0.3)

    parent.Children.value.append(sphere)
    parent.Children.value.append(plane)
    parent.Children.value.append(monkey)

  # physics / lights example
  physics = avango.gua.nodes.Physics()
  physics.add_rigid_body(oilrig_rb)

  light_proxy_size = 0.2
  light_size = 2.0

  avango.gua.create_sphere_shape("sphere", light_proxy_size*0.5)

  def add_lights():

    physics.State.value = avango.gua.PhysicsRunningState.STOPPED

    physics.Gravity.value = avango.gua.Vec3(0, 0, 0)

    loader = avango.gua.nodes.GeometryLoader()

    count = 200

    for x in range(0, count):

      color = avango.gua.Color(1.0, 0.5, 0.5)
      material = "Light3"

      if x%3 == 0:
        color = avango.gua.Color(0.5, 1.0, 0.5)
        material = "Light2"
      elif x%3 == 1:
        color = avango.gua.Color(0.5, 0.5, 1.0)
        material = "Light1"

      light_csn = avango.gua.nodes.CollisionShapeNode(
        Name = "light_csn",
        ShapeName = "sphere",
        Transform = avango.gua.make_identity_mat()
      )

      randdir = avango.gua.Vec3(
        5*(random.random() * 2.0 - 1.0) + 8,
        3*(random.random() * 2.0 - 1.0) + 6,
        5*(random.random() * 2.0 - 1.0) + 0
      )

      sphere_geometry = loader.create_geometry_from_file(
        "sphere",
        "data/objects/light_sphere.obj",
        material,
        avango.gua.LoaderFlags.DEFAULTS
      )
      sphere_geometry.Transform.value = avango.gua.make_scale_mat(light_proxy_size, light_proxy_size, light_proxy_size)

      point_light = avango.gua.nodes.PointLightNode(
        Name = "light",
        Color = color,
        Transform = avango.gua.make_scale_mat(light_size, light_size, light_size)
      )

      light_rb = avango.gua.nodes.RigidBodyNode(
        Name = "light_rb" + str(x),
        Mass = 0.5,
        Friction = 0.6,
        RollingFriction = 0.03,
        AngularDamping = 0.1,
        LinearDamping = 0.1,
        Restitution = 0.08,
        Transform = avango.gua.make_trans_mat(randdir),
        Children = [light_csn, point_light]
      )

      light_csn.Children.value = [sphere_geometry]


      graph.Root.value.Children.value.append(light_rb)
      physics.add_rigid_body(light_rb)

    physics.State.value = avango.gua.PhysicsRunningState.RUNNING


  def drop_lights():
    physics.Gravity.value = avango.gua.Vec3(0, -1.7, 0)


  def _display_materials(parent):

    mats = {
      "SimplePhong" : ["SimplePhongGreen", "SimplePhongRed", "SimplePhongWhite"],
      "Planet" : ["PlanetPink", "PlanetYellow", "PlanetGray"],
      "ComplexPhong" : ["ComplexPhongStone", "ComplexPhongWood", "Tiles"]
    }

    x = 0;
    y = 0;

    for materials in mats.values():
      for material in materials:
        _display_material(parent, x-1, y-1, material);
        y += 1
      x += 1
      y = 0

  _display_materials(material_example_group);

  ##############################################################################
  ## navigation
  ##############################################################################

  slide_transform.Children.value = [slides]
  navigation.Children.value = [screen, view, slide_transform]

  graph.Root.value.Children.value = [oilrig_rb, water, sun, fake_sun, navigation]

  pipe = setup_pipe()

  navigator = examples_common.navigator.Navigator()
  #navigator.Keyboard.set_device_number(0)
  navigator.StartLocation.value = navigation.Transform.value.get_translate()
  navigator.StartRotation.value = avango.gua.Vec2(0, 790)
  navigator.OutTransform.connect_from(navigation.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  # enable navigation
  # navigation.Transform.connect_from(navigator.OutTransform)

  slide_switcher = SlideSwitcher()
  slide_switcher.LastSlide.value = 15
  slide_switcher.NextSlide.connect_from(navigator.Keyboard.KeyUp)
  slide_switcher.PreviousSlide.connect_from(navigator.Keyboard.KeyDown)
  slide_switcher.LastSlide.value = 26
  slide_switcher.SlideLocation.value = slide_transform.Transform.value.get_translate()
  slide_switcher.SlideYRotation.value = -90
  slide_switcher.SlideOffset.value = SLIDE_OFFSET

  slide_switcher.OutTransform.connect_from(slide_transform.Transform)
  slide_transform.Transform.connect_from(slide_switcher.OutTransform)

  slide_transform.Children.value.append(avango_example)
  slide_transform.Children.value.append(different_kinds_of_data)
  slide_transform.Children.value.append(material_example)

  ##############################################################################
  ## fancy scene setup
  ##############################################################################
  def make_presentation():
    slide_switcher.TransitionSmoothness.value = 0
    slide_switcher.increase_current_slide()

  def make_awesome():
    pipe.EnableFrustumCulling.value = True
    #slide_switcher.SlideLocation.value = avango.gua.Vec3(0, 0, -2.37)
    slide_switcher.TransitionSmoothness.value = 0.7
    pipe.AmbientColor.value = avango.gua.Color(0.6, 0.6, 1.0)
    view.Children.value = [head_light]
    slide_switcher.CurrentSlide.value = 3

  def begin_guacamole():
    make_awesome()

  def fly_down():

    final_pos = avango.gua.make_trans_mat(6, -28, -25) * avango.gua.make_rot_mat(150, -0.1, 0.9, 0.4)

    anim = Animation(
      Duration = 10000,
      StartPos = navigation.Transform.value.get_translate(),
      StartRot = navigation.Transform.value.get_rotate(),
      EndPos = final_pos.get_translate(),
      EndRot = final_pos.get_rotate()
    )
    navigation.Transform.connect_from(anim.CurrentMatrix)

  def disable_effects():
    pipe.EnableSsao.value = False
    pipe.EnableBloom.value = False
    pipe.EnableFog.value = False
    sun.EnableShadows.value = False
    fake_sun.EnableGodrays.value = False

  def enable_effects():
    pipe.EnableSsao.value = True
    pipe.EnableBloom.value = True
    pipe.EnableFog.value = True
    pipe.EnableFXAA.value = True
    sun.EnableShadows.value = True
    fake_sun.EnableGodrays.value = True

  def change_some_materials():
    avango.gua.set_material_uniform("SimplePhongWhite", "diffuse_color", avango.gua.Vec3(1, 0, 0))
    avango.gua.set_material_uniform("SimplePhongRed", "diffuse_color", avango.gua.Vec3(1, 1, 0))
    avango.gua.set_material_uniform("SimplePhongGreen", "diffuse_color", avango.gua.Vec3(1, 0, 1))

  def toggle_ssao():
    pipe.EnableSsao.value = not (pipe.EnableSsao.value)

  def toggle_fxaa():
    pipe.EnableFXAA.value = not (pipe.EnableFXAA.value)

  def toggle_bloom():
    pipe.EnableBloom.value = not (pipe.EnableBloom.value)

  def toggle_shadows():
    sun.EnableShadows.value = not (sun.EnableShadows.value)

  def toggle_volumetric_light():
    toggle_godrays()

  def toggle_godrays():
    fake_sun.EnableGodrays.value = not (fake_sun.EnableGodrays.value)

  def toggle_fog():
    pipe.EnableFog.value = not (pipe.EnableFog.value)

  def show_buffers():
    pipe.EnablePreviewDisplay.value = True

  def hide_buffers():
    pipe.EnablePreviewDisplay.value = False

  def next_slide():
    slide_switcher.TransitionSmoothness.value = 0.9
    slide_switcher.increase_current_slide()

  def previous_slide():
    slide_switcher.TransitionSmoothness.value = 0.9
    slide_switcher.decrease_current_slide()

  def goto_slide(i):
    slide_switcher.goto_slide(i)


  water_updater = TimedMaterialUniformUpdate()
  water_updater.MaterialName.value = "Water"
  water_updater.UniformName.value = "time"
  water_updater.TimeIn.connect_from(timer.Time)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]
  viewer.Physics.value = physics

  viewer.run()


if __name__ == '__main__':
  start()

