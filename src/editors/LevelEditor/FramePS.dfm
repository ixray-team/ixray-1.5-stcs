object fraPS: TfraPS
  Left = 344
  Top = 335
  VertScrollBar.Visible = False
  Align = alClient
  BorderStyle = bsNone
  ClientHeight = 250
  ClientWidth = 217
  Color = 10528425
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object paSelectObject: TPanel
    Left = 0
    Top = 0
    Width = 217
    Height = 50
    Align = alTop
    ParentColor = True
    TabOrder = 0
    object Label5: TLabel
      Left = 2
      Top = 16
      Width = 58
      Height = 13
      Caption = 'Ref'#39's select:'
    end
    object ebSelectByRefs: TExtBtn
      Left = 100
      Top = 16
      Width = 47
      Height = 15
      Align = alNone
      BtnColor = 10528425
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = False
      OnClick = ebSelectByRefsClick
    end
    object ebDeselectByRefs: TExtBtn
      Left = 148
      Top = 16
      Width = 47
      Height = 15
      Align = alNone
      BtnColor = 10528425
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = False
      OnClick = ebDeselectByRefsClick
    end
    object Label: TLabel
      Left = 1
      Top = 1
      Width = 215
      Height = 13
      Align = alTop
      Alignment = taCenter
      Caption = 'PS Commands'
      Color = clGray
      ParentColor = False
      OnClick = ExpandClick
    end
    object ExtBtn3: TExtBtn
      Left = 205
      Top = 2
      Width = 11
      Height = 11
      Align = alNone
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        DE000000424DDE00000000000000360000002800000007000000070000000100
        180000000000A8000000120B0000120B00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF00
        0000FFFFFFFFFFFF000000000000000000FFFFFFFFFFFF000000FFFFFF000000
        000000000000000000000000FFFFFF0000000000000000000000000000000000
        00000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00
        0000}
      ParentFont = False
      OnClick = PaneMinClick
    end
    object ebCurrentPSPlay: TExtBtn
      Left = 100
      Top = 32
      Width = 47
      Height = 15
      Align = alNone
      BtnColor = 10528425
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        C2010000424DC20100000000000036000000280000000C0000000B0000000100
        1800000000008C010000120B0000120B00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFEBEBEBA7A7A7E0E0E0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFBABABA0000005050508E8E8EEDEDEDFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFBABABA0000000000000000004F4F4F7B7B7BFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFBABABA00000000000000000000000008080828
        2828898989FFFFFFFFFFFFFFFFFFFFFFFFBABABA000000000000000000000000
        0000000000001212121F1F1FC6C6C6FFFFFFFFFFFFBABABA0000000000000000
        00000000080808282828898989FFFFFFFFFFFFFFFFFFFFFFFFBABABA00000000
        00000000004F4F4F7B7B7BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBABABA
        0000005050508E8E8EEDEDEDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFEBEBEBA7A7A7E0E0E0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF}
      ParentFont = False
      Transparent = False
      OnClick = ebCurrentPSPlayClick
    end
    object ebCurrentPSStop: TExtBtn
      Left = 148
      Top = 32
      Width = 47
      Height = 15
      Align = alNone
      BtnColor = 10528425
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Glyph.Data = {
        9E010000424D9E0100000000000036000000280000000C0000000A0000000100
        18000000000068010000120B0000120B00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFDDDDDDA5A5A5ADADADADADADADADADADADADA5A5A5DDDDDDFFFFFFFF
        FFFFFFFFFFFFFFFF969696000000000000000000000000000000000000969696
        FFFFFFFFFFFFFFFFFFFFFFFF9696960000000000000000000000000000000000
        00969696FFFFFFFFFFFFFFFFFFFFFFFF96969600000000000000000000000000
        0000000000969696FFFFFFFFFFFFFFFFFFFFFFFF969696000000000000000000
        000000000000000000969696FFFFFFFFFFFFFFFFFFFFFFFF9696960000000000
        00000000000000000000000000969696FFFFFFFFFFFFFFFFFFFFFFFF96969600
        0000000000000000000000000000000000969696FFFFFFFFFFFFFFFFFFFFFFFF
        DDDDDDA5A5A5ADADADADADADADADADADADADA5A5A5DDDDDDFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFF}
      ParentFont = False
      Transparent = False
      OnClick = ebCurrentPSStopClick
    end
    object Label1: TLabel
      Left = 2
      Top = 32
      Width = 45
      Height = 13
      Caption = 'Selected:'
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 50
    Width = 217
    Height = 200
    Align = alClient
    Constraints.MinHeight = 175
    ParentColor = True
    TabOrder = 1
    object APHeadLabel2: TLabel
      Left = 1
      Top = 1
      Width = 215
      Height = 13
      Align = alTop
      Alignment = taCenter
      Caption = 'Current PS'
      Color = clGray
      ParentColor = False
      OnClick = ExpandClick
    end
    object Bevel1: TBevel
      Left = 1
      Top = 14
      Width = 215
      Height = 1
      Align = alTop
      Shape = bsSpacer
    end
    object paItems: TPanel
      Left = 1
      Top = 15
      Width = 215
      Height = 184
      Align = alClient
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
    end
  end
  object fsStorage: TFormStorage
    IniSection = 'FramePS'
    Options = []
    Version = 12
    StoredProps.Strings = (
      'paSelectObject.Height'
      'paSelectObject.Tag'
      'Panel1.Height'
      'Panel1.Tag')
    StoredValues = <>
    Top = 65528
  end
  object ilModeIcons: TImageList
    Height = 14
    Width = 9
    Top = 24
    Bitmap = {
      494C010103000400040009000E00FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000240000000E0000000100200000000000E007
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      28000000240000000E0000000100010000000000700000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFE000000000FFFFFFE000000000
      FFFFFFE00000000079BC5F00000000007EBDDEC0000000001D8CC68000000000
      6BB5DAE0000000001C8C470000000000FFFFFFE000000000FFFFFFE000000000
      FFFFFFE000000000FFFFFFE000000000FFFFFFE000000000FFFFFFE000000000
      00000000000000000000000000000000000000000000}
  end
end
