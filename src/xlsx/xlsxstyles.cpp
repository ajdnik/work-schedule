/****************************************************************************
** Copyright (c) 2013 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
#include "xlsxstyles_p.h"
#include "xlsxxmlwriter_p.h"
#include "xlsxxmlreader_p.h"
#include "xlsxformat_p.h"
#include "xlsxutility_p.h"
#include <QFile>
#include <QMap>
#include <QDataStream>
#include <QDebug>
#include <QBuffer>

namespace QXlsx {

/*
  When loading from existing .xlsx file. we should create a clean styles object.
  otherwise, default formats should be added.

*/
Styles::Styles(bool createEmpty)
    : m_nextCustomNumFmtId(176), m_emptyFormatAdded(false)
{
    //!Fix me. Should the custom num fmt Id starts with 164 or 176 or others??

    if (!createEmpty) {
        //Add default Format
        Format defaultFmt;
        addFormat(defaultFmt);

        //Add another fill format
        Format fillFmt;
        fillFmt.setFillPattern(Format::PatternGray125);
        m_fillsList.append(fillFmt);
        m_fillsHash.insert(fillFmt.fillKey(), fillFmt);
    }
}

Styles::~Styles()
{
}

Format Styles::xfFormat(int idx) const
{
    if (idx <0 || idx >= m_xf_formatsList.size())
        return Format();

    return m_xf_formatsList[idx];
}

/*
   Assign index to Font/Fill/Border and Format

   When \a force is true, add the format to the format list, even other format has
   the same key have been in.
   This is useful when reading existing .xlsx files which may contains duplicated formats.
*/
void Styles::addFormat(const Format &format, bool force)
{
    if (format.isEmpty()) {
        //Try do something for empty Format.
        if (m_emptyFormatAdded)
            return;
        m_emptyFormatAdded = true;
    }

    //numFmt
    if (format.hasNumFmtData() && !format.hasProperty(FormatPrivate::P_NumFmt_Id)) {
        if (m_builtinNumFmtsHash.isEmpty()) {
            m_builtinNumFmtsHash.insert(QStringLiteral("General"), 0);
            m_builtinNumFmtsHash.insert(QStringLiteral("0"), 1);
            m_builtinNumFmtsHash.insert(QStringLiteral("0.00"), 2);
            m_builtinNumFmtsHash.insert(QStringLiteral("#,##0"), 3);
            m_builtinNumFmtsHash.insert(QStringLiteral("#,##0.00"), 4);
//            m_builtinNumFmtsHash.insert(QStringLiteral("($#,##0_);($#,##0)"), 5);
//            m_builtinNumFmtsHash.insert(QStringLiteral("($#,##0_);[Red]($#,##0)"), 6);
//            m_builtinNumFmtsHash.insert(QStringLiteral("($#,##0.00_);($#,##0.00)"), 7);
//            m_builtinNumFmtsHash.insert(QStringLiteral("($#,##0.00_);[Red]($#,##0.00)"), 8);
            m_builtinNumFmtsHash.insert(QStringLiteral("0%"), 9);
            m_builtinNumFmtsHash.insert(QStringLiteral("0.00%"), 10);
            m_builtinNumFmtsHash.insert(QStringLiteral("0.00E+00"), 11);
            m_builtinNumFmtsHash.insert(QStringLiteral("# ?/?"), 12);
            m_builtinNumFmtsHash.insert(QStringLiteral("# ?\?/??"), 13);// Note: "??/" is a c++ trigraph, so escape one "?"
            m_builtinNumFmtsHash.insert(QStringLiteral("m/d/yy"), 14);
            m_builtinNumFmtsHash.insert(QStringLiteral("d-mmm-yy"), 15);
            m_builtinNumFmtsHash.insert(QStringLiteral("d-mmm"), 16);
            m_builtinNumFmtsHash.insert(QStringLiteral("mmm-yy"), 17);
            m_builtinNumFmtsHash.insert(QStringLiteral("h:mm AM/PM"), 18);
            m_builtinNumFmtsHash.insert(QStringLiteral("h:mm:ss AM/PM"), 19);
            m_builtinNumFmtsHash.insert(QStringLiteral("h:mm"), 20);
            m_builtinNumFmtsHash.insert(QStringLiteral("h:mm:ss"), 21);
            m_builtinNumFmtsHash.insert(QStringLiteral("m/d/yy h:mm"), 22);

            m_builtinNumFmtsHash.insert(QStringLiteral("(#,##0_);(#,##0)"), 37);
            m_builtinNumFmtsHash.insert(QStringLiteral("(#,##0_);[Red](#,##0)"), 38);
            m_builtinNumFmtsHash.insert(QStringLiteral("(#,##0.00_);(#,##0.00)"), 39);
            m_builtinNumFmtsHash.insert(QStringLiteral("(#,##0.00_);[Red](#,##0.00)"), 40);
//            m_builtinNumFmtsHash.insert(QStringLiteral("_(* #,##0_);_(* (#,##0);_(* \"-\"_);_(_)"), 41);
//            m_builtinNumFmtsHash.insert(QStringLiteral("_($* #,##0_);_($* (#,##0);_($* \"-\"_);_(_)"), 42);
//            m_builtinNumFmtsHash.insert(QStringLiteral("_(* #,##0.00_);_(* (#,##0.00);_(* \"-\"??_);_(_)"), 43);
//            m_builtinNumFmtsHash.insert(QStringLiteral("_($* #,##0.00_);_($* (#,##0.00);_($* \"-\"??_);_(_)"), 44);
            m_builtinNumFmtsHash.insert(QStringLiteral("mm:ss"), 45);
            m_builtinNumFmtsHash.insert(QStringLiteral("[h]:mm:ss"), 46);
            m_builtinNumFmtsHash.insert(QStringLiteral("mm:ss.0"), 47);
            m_builtinNumFmtsHash.insert(QStringLiteral("##0.0E+0"), 48);
            m_builtinNumFmtsHash.insert(QStringLiteral("@"), 49);
        }
        const QString str = format.numberFormat();
        //Assign proper number format index
        if (m_builtinNumFmtsHash.contains(str)) {
            const_cast<Format *>(&format)->setNumberFormat(m_builtinNumFmtsHash[str], str);
        } else if (m_customNumFmtsHash.contains(str)) {
            const_cast<Format *>(&format)->setNumberFormat(m_customNumFmtsHash[str]->formatIndex, str);
        } else {
            //Assign a new fmt Id.
            const_cast<Format *>(&format)->setNumberFormat(m_nextCustomNumFmtId, str);

            QSharedPointer<XlsxFormatNumberData> fmt(new XlsxFormatNumberData);
            fmt->formatIndex = m_nextCustomNumFmtId;
            fmt->formatString = str;
            m_customNumFmtIdMap.insert(m_nextCustomNumFmtId, fmt);
            m_customNumFmtsHash.insert(str, fmt);

            m_nextCustomNumFmtId += 1;
        }
    }

    //Font
    if (format.hasFontData() && !format.fontIndexValid()) {
        //Assign proper font index, if has font data.
        if (!m_fontsHash.contains(format.fontKey()))
            const_cast<Format *>(&format)->setFontIndex(m_fontsList.size());
        else
            const_cast<Format *>(&format)->setFontIndex(m_fontsHash[format.fontKey()].fontIndex());
    }
    if (!m_fontsHash.contains(format.fontKey())) {
        //Still a valid font if the format has no fontData. (All font properties are default)
        m_fontsList.append(format);
        m_fontsHash[format.fontKey()] = format;
    }

    //Fill
    if (format.hasFillData() && !format.fillIndexValid()) {
        //Assign proper fill index, if has fill data.
        if (!m_fillsHash.contains(format.fillKey()))
            const_cast<Format *>(&format)->setFillIndex(m_fillsList.size());
        else
            const_cast<Format *>(&format)->setFillIndex(m_fillsHash[format.fillKey()].fillIndex());
    }
    if (!m_fillsHash.contains(format.fillKey())) {
        //Still a valid fill if the format has no fillData. (All fill properties are default)
        m_fillsList.append(format);
        m_fillsHash[format.fillKey()] = format;
    }

    //Border
    if (format.hasBorderData() && !format.borderIndexValid()) {
        //Assign proper border index, if has border data.
        if (!m_bordersHash.contains(format.borderKey()))
            const_cast<Format *>(&format)->setBorderIndex(m_bordersList.size());
        else
            const_cast<Format *>(&format)->setBorderIndex(m_bordersHash[format.borderKey()].borderIndex());
    }
    if (!m_bordersHash.contains(format.borderKey())) {
        //Still a valid border if the format has no borderData. (All border properties are default)
        m_bordersList.append(format);
        m_bordersHash[format.borderKey()] = format;
    }

    //Format
//    if (format.isDxfFormat()) {
//        if (!format.dxfIndexValid()) {
//            if (!m_dxf_formatsHash.contains(format.formatKey())) {
//                const_cast<Format *>(&format)->setDxfIndex(m_dxf_formatsList.size());
//                m_dxf_formatsList.append(format);
//                m_dxf_formatsHash[format.formatKey()] = format;
//            } else {
//                const_cast<Format *>(&format)->setDxfIndex(m_dxf_formatsHash[format.formatKey()].dxfIndex());
//            }
//        }
//    } else {
    if (!format.isEmpty() && !format.xfIndexValid()) {
        if (m_xf_formatsHash.contains(format.formatKey()))
            const_cast<Format *>(&format)->setXfIndex(m_xf_formatsHash[format.formatKey()].xfIndex());
        else
            const_cast<Format *>(&format)->setXfIndex(m_xf_formatsList.size());
    }
    if (!m_xf_formatsHash.contains(format.formatKey()) || force) {
        m_xf_formatsList.append(format);
        m_xf_formatsHash[format.formatKey()] = format;
    }
//    }
}

QByteArray Styles::saveToXmlData()
{
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    saveToXmlFile(&buffer);

    return data;
}

void Styles::saveToXmlFile(QIODevice *device)
{
    XmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("styleSheet"));
    writer.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://schemas.openxmlformats.org/spreadsheetml/2006/main"));

    writeNumFmts(writer);
    writeFonts(writer);
    writeFills(writer);
    writeBorders(writer);

    writer.writeStartElement(QStringLiteral("cellStyleXfs"));
    writer.writeAttribute(QStringLiteral("count"), QStringLiteral("1"));
    writer.writeStartElement(QStringLiteral("xf"));
    writer.writeAttribute(QStringLiteral("numFmtId"), QStringLiteral("0"));
    writer.writeAttribute(QStringLiteral("fontId"), QStringLiteral("0"));
    writer.writeAttribute(QStringLiteral("fillId"), QStringLiteral("0"));
    writer.writeAttribute(QStringLiteral("borderId"), QStringLiteral("0"));
    writer.writeEndElement();//xf
    writer.writeEndElement();//cellStyleXfs

    writeCellXfs(writer);

    writer.writeStartElement(QStringLiteral("cellStyles"));
    writer.writeAttribute(QStringLiteral("count"), QStringLiteral("1"));
    writer.writeStartElement(QStringLiteral("cellStyle"));
    writer.writeAttribute(QStringLiteral("name"), QStringLiteral("Normal"));
    writer.writeAttribute(QStringLiteral("xfId"), QStringLiteral("0"));
    writer.writeAttribute(QStringLiteral("builtinId"), QStringLiteral("0"));
    writer.writeEndElement();//cellStyle
    writer.writeEndElement();//cellStyles

    writeDxfs(writer);

    writer.writeStartElement(QStringLiteral("tableStyles"));
    writer.writeAttribute(QStringLiteral("count"), QStringLiteral("0"));
    writer.writeAttribute(QStringLiteral("defaultTableStyle"), QStringLiteral("TableStyleMedium9"));
    writer.writeAttribute(QStringLiteral("defaultPivotStyle"), QStringLiteral("PivotStyleLight16"));
    writer.writeEndElement();//tableStyles

    writer.writeEndElement();//styleSheet
    writer.writeEndDocument();
}

void Styles::writeNumFmts(XmlStreamWriter &writer)
{
    if (m_customNumFmtIdMap.size() == 0)
        return;

    writer.writeStartElement(QStringLiteral("numFmts"));
    writer.writeAttribute(QStringLiteral("count"), QString::number(m_customNumFmtIdMap.count()));

    QMapIterator<int, QSharedPointer<XlsxFormatNumberData> > it(m_customNumFmtIdMap);
    while(it.hasNext()) {
        it.next();
        writer.writeEmptyElement(QStringLiteral("numFmt"));
        writer.writeAttribute(QStringLiteral("numFmtId"), QString::number(it.value()->formatIndex));
        writer.writeAttribute(QStringLiteral("formatCode"), it.value()->formatString);
    }
    writer.writeEndElement();//numFmts
}

/*
 not consider dxf format.
*/
void Styles::writeFonts(XmlStreamWriter &writer)
{
    writer.writeStartElement(QStringLiteral("fonts"));
    writer.writeAttribute(QStringLiteral("count"), QString::number(m_fontsList.count()));
    for (int i=0; i<m_fontsList.size(); ++i) {
        const Format &format = m_fontsList[i];
        writer.writeStartElement(QStringLiteral("font"));

        if (format.fontBold())
            writer.writeEmptyElement(QStringLiteral("b"));
        if (format.fontItalic())
            writer.writeEmptyElement(QStringLiteral("i"));
        if (format.fontStrikeOut())
            writer.writeEmptyElement(QStringLiteral("strike"));
        if (format.fontOutline())
            writer.writeEmptyElement(QStringLiteral("outline"));
        if (format.boolProperty(FormatPrivate::P_Font_Shadow))
            writer.writeEmptyElement(QStringLiteral("shadow"));
        if (format.hasProperty(FormatPrivate::P_Font_Underline)) {
            Format::FontUnderline u = format.fontUnderline();
            if (u != Format::FontUnderlineNone) {
                writer.writeEmptyElement(QStringLiteral("u"));
                if (u== Format::FontUnderlineDouble)
                    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("double"));
                else if (u == Format::FontUnderlineSingleAccounting)
                    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("singleAccounting"));
                else if (u == Format::FontUnderlineDoubleAccounting)
                    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("doubleAccounting"));
            }
        }
        if (format.hasProperty(FormatPrivate::P_Font_Script)) {
            Format::FontScript s = format.fontScript();
            if (s != Format::FontScriptNormal) {
                writer.writeEmptyElement(QStringLiteral("vertAlign"));
                if (s == Format::FontScriptSuper)
                    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("superscript"));
                else
                    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("subscript"));
            }
        }

        if (format.hasProperty(FormatPrivate::P_Font_Size)) {
            writer.writeEmptyElement(QStringLiteral("sz"));
            writer.writeAttribute(QStringLiteral("val"), QString::number(format.fontSize()));
        }

        if (format.fontColor().isValid()) {
            writer.writeEmptyElement(QStringLiteral("color"));
            QString color = format.fontColor().name();
            writer.writeAttribute(QStringLiteral("rgb"), QStringLiteral("FF")+color.mid(1));//remove #
        } else if (format.hasProperty(FormatPrivate::P_Font_ThemeColor)) {
            writer.writeEmptyElement(QStringLiteral("color"));
            QStringList themes = format.stringProperty(FormatPrivate::P_Font_ThemeColor).split(QLatin1Char(':'));
            writer.writeAttribute(QStringLiteral("theme"), themes[0]);
            if (!themes[1].isEmpty())
                writer.writeAttribute(QStringLiteral("tint"), themes[1]);
        }

        if (!format.fontName().isEmpty()) {
            writer.writeEmptyElement(QStringLiteral("name"));
            writer.writeAttribute(QStringLiteral("val"), format.fontName());
        }
        if (format.hasProperty(FormatPrivate::P_Font_Family)) {
            writer.writeEmptyElement(QStringLiteral("family"));
            writer.writeAttribute(QStringLiteral("val"), QString::number(format.intProperty(FormatPrivate::P_Font_Family)));
        }

        if (format.hasProperty(FormatPrivate::P_Font_Scheme)) {
            writer.writeEmptyElement(QStringLiteral("scheme"));
            writer.writeAttribute(QStringLiteral("val"), format.stringProperty(FormatPrivate::P_Font_Scheme));
        }

//        if (!format.isDxfFormat()) {
//            writer.writeEmptyElement(QStringLiteral("sz"));
//            writer.writeAttribute(QStringLiteral("val"), QString::number(format.fontSize()));
//        }
//        if (!format.isDxfFormat()) {
//            writer.writeEmptyElement(QStringLiteral("name"));
//            writer.writeAttribute(QStringLiteral("val"), format.fontName());
//            writer.writeEmptyElement(QStringLiteral("family"));
//            writer.writeAttribute(QStringLiteral("val"), QString::number(format.fontFamily()));
//            if (format.fontName() == QLatin1String("Calibri")) {
//                writer.writeEmptyElement(QStringLiteral("scheme"));
//                writer.writeAttribute(QStringLiteral("val"), format.fontScheme());
//            }
//        }
        writer.writeEndElement(); //font
    }
    writer.writeEndElement();//fonts
}

void Styles::writeFills(XmlStreamWriter &writer)
{
    writer.writeStartElement(QStringLiteral("fills"));
    writer.writeAttribute(QStringLiteral("count"), QString::number(m_fillsList.size()));

    for (int i=0; i<m_fillsList.size(); ++i)
        writeFill(writer, m_fillsList[i]);

    writer.writeEndElement(); //fills
}

void Styles::writeFill(XmlStreamWriter &writer, const Format &fill)
{
    static QMap<int, QString> patternStrings;
    if (patternStrings.isEmpty()) {
        patternStrings[Format::PatternNone] = QStringLiteral("none");
        patternStrings[Format::PatternSolid] = QStringLiteral("solid");
        patternStrings[Format::PatternMediumGray] = QStringLiteral("mediumGray");
        patternStrings[Format::PatternDarkGray] = QStringLiteral("darkGray");
        patternStrings[Format::PatternLightGray] = QStringLiteral("lightGray");
        patternStrings[Format::PatternDarkHorizontal] = QStringLiteral("darkHorizontal");
        patternStrings[Format::PatternDarkVertical] = QStringLiteral("darkVertical");
        patternStrings[Format::PatternDarkDown] = QStringLiteral("darkDown");
        patternStrings[Format::PatternDarkUp] = QStringLiteral("darkUp");
        patternStrings[Format::PatternDarkGrid] = QStringLiteral("darkGrid");
        patternStrings[Format::PatternDarkTrellis] = QStringLiteral("darkTrellis");
        patternStrings[Format::PatternLightHorizontal] = QStringLiteral("lightHorizontal");
        patternStrings[Format::PatternLightVertical] = QStringLiteral("lightVertical");
        patternStrings[Format::PatternLightDown] = QStringLiteral("lightDown");
        patternStrings[Format::PatternLightUp] = QStringLiteral("lightUp");
        patternStrings[Format::PatternLightTrellis] = QStringLiteral("lightTrellis");
        patternStrings[Format::PatternGray125] = QStringLiteral("gray125");
        patternStrings[Format::PatternGray0625] = QStringLiteral("gray0625");
        patternStrings[Format::PatternLightGrid] = QStringLiteral("lightGrid");
    }

    writer.writeStartElement(QStringLiteral("fill"));
    writer.writeStartElement(QStringLiteral("patternFill"));
    writer.writeAttribute(QStringLiteral("patternType"), patternStrings[fill.fillPattern()]);
    // For a solid fill, Excel reverses the role of foreground and background colours
    if (fill.patternForegroundColor().isValid()) {
        writer.writeEmptyElement(fill.fillPattern() == Format::PatternSolid ? QStringLiteral("bgColor") : QStringLiteral("fgColor"));
        writer.writeAttribute(QStringLiteral("rgb"), QStringLiteral("FF")+fill.patternForegroundColor().name().mid(1));
    } else if (!fill.stringProperty(FormatPrivate::P_Fill_FgThemeColor).isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("fgColor"));
        QStringList themes = fill.stringProperty(FormatPrivate::P_Fill_FgThemeColor).split(QLatin1Char(':'));
        writer.writeAttribute(QStringLiteral("theme"), themes[0]);
        if (!themes[1].isEmpty())
            writer.writeAttribute(QStringLiteral("tint"), themes[1]);
    }
    if (fill.patternBackgroundColor().isValid()) {
        writer.writeEmptyElement(fill.fillPattern() == Format::PatternSolid ? QStringLiteral("fgColor") : QStringLiteral("bgColor"));
        writer.writeAttribute(QStringLiteral("rgb"), QStringLiteral("FF")+fill.patternBackgroundColor().name().mid(1));
    } else if (!fill.stringProperty(FormatPrivate::P_Fill_BgThemeColor).isEmpty()) {
        writer.writeEmptyElement(QStringLiteral("bgColor"));
        QStringList themes = fill.stringProperty(FormatPrivate::P_Fill_BgThemeColor).split(QLatin1Char(':'));
        writer.writeAttribute(QStringLiteral("theme"), themes[0]);
        if (!themes[1].isEmpty())
            writer.writeAttribute(QStringLiteral("tint"), themes[1]);
    }

    writer.writeEndElement();//patternFill
    writer.writeEndElement();//fill
}

void Styles::writeBorders(XmlStreamWriter &writer)
{
    writer.writeStartElement(QStringLiteral("borders"));
    writer.writeAttribute(QStringLiteral("count"), QString::number(m_bordersList.count()));
    for (int i=0; i<m_bordersList.size(); ++i) {
        const Format &border = m_bordersList[i];

        writer.writeStartElement(QStringLiteral("border"));
        if (border.hasProperty(FormatPrivate::P_Border_DiagonalType)) {
            Format::DiagonalBorderType t = border.diagonalBorderType();
            if (t == Format::DiagonalBorderUp) {
                writer.writeAttribute(QStringLiteral("diagonalUp"), QStringLiteral("1"));
            } else if (t == Format::DiagonalBorderDown) {
                writer.writeAttribute(QStringLiteral("diagonalDown"), QStringLiteral("1"));
            } else if (t == Format::DiagnoalBorderBoth) {
                writer.writeAttribute(QStringLiteral("diagonalUp"), QStringLiteral("1"));
                writer.writeAttribute(QStringLiteral("diagonalDown"), QStringLiteral("1"));
            }
        }
        if (border.hasProperty(FormatPrivate::P_Border_LeftStyle))
            writeSubBorder(writer, QStringLiteral("left"), border.leftBorderStyle(), border.leftBorderColor(), border.stringProperty(FormatPrivate::P_Border_ThemeLeftColor));
        if (border.hasProperty(FormatPrivate::P_Border_RightStyle))
            writeSubBorder(writer, QStringLiteral("right"), border.rightBorderStyle(), border.rightBorderColor(), border.stringProperty(FormatPrivate::P_Border_ThemeRightColor));
        if (border.hasProperty(FormatPrivate::P_Border_TopStyle))
            writeSubBorder(writer, QStringLiteral("top"), border.topBorderStyle(), border.topBorderColor(), border.stringProperty(FormatPrivate::P_Border_ThemeTopColor));
        if (border.hasProperty(FormatPrivate::P_Border_BottomStyle))
            writeSubBorder(writer, QStringLiteral("bottom"), border.bottomBorderStyle(), border.bottomBorderColor(), border.stringProperty(FormatPrivate::P_Border_ThemeBottomColor));

//        if (!format.isDxfFormat()) {
        if (border.hasProperty(FormatPrivate::P_Border_DiagonalStyle))
            writeSubBorder(writer, QStringLiteral("diagonal"), border.diagonalBorderStyle(), border.diagonalBorderColor(), border.stringProperty(FormatPrivate::P_Border_ThemeDiagonalColor));
//        }
        writer.writeEndElement();//border
    }
    writer.writeEndElement();//borders
}

void Styles::writeSubBorder(XmlStreamWriter &writer, const QString &type, int style, const QColor &color, const QString &themeColor)
{
    if (style == Format::BorderNone) {
        writer.writeEmptyElement(type);
        return;
    }

    static QMap<int, QString> stylesString;
    if (stylesString.isEmpty()) {
        stylesString[Format::BorderNone] = QStringLiteral("none");
        stylesString[Format::BorderThin] = QStringLiteral("thin");
        stylesString[Format::BorderMedium] = QStringLiteral("medium");
        stylesString[Format::BorderDashed] = QStringLiteral("dashed");
        stylesString[Format::BorderDotted] = QStringLiteral("dotted");
        stylesString[Format::BorderThick] = QStringLiteral("thick");
        stylesString[Format::BorderDouble] = QStringLiteral("double");
        stylesString[Format::BorderHair] = QStringLiteral("hair");
        stylesString[Format::BorderMediumDashed] = QStringLiteral("mediumDashed");
        stylesString[Format::BorderDashDot] = QStringLiteral("dashDot");
        stylesString[Format::BorderMediumDashDot] = QStringLiteral("mediumDashDot");
        stylesString[Format::BorderDashDotDot] = QStringLiteral("dashDotDot");
        stylesString[Format::BorderMediumDashDotDot] = QStringLiteral("mediumDashDotDot");
        stylesString[Format::BorderSlantDashDot] = QStringLiteral("slantDashDot");
    }

    writer.writeStartElement(type);
    writer.writeAttribute(QStringLiteral("style"), stylesString[style]);
    writer.writeEmptyElement(QStringLiteral("color"));
    if (color.isValid()) {
        writer.writeAttribute(QStringLiteral("rgb"), QStringLiteral("FF")+color.name().mid(1)); //remove #
    } else if (!themeColor.isEmpty()) {
        QStringList themes = themeColor.split(QLatin1Char(':'));
        writer.writeAttribute(QStringLiteral("theme"), themes[0]);
        if (!themes[1].isEmpty())
            writer.writeAttribute(QStringLiteral("tint"), themes[1]);
    } else {
        writer.writeAttribute(QStringLiteral("auto"), QStringLiteral("1"));
    }
    writer.writeEndElement();//type
}

void Styles::writeCellXfs(XmlStreamWriter &writer)
{
    writer.writeStartElement(QStringLiteral("cellXfs"));
    writer.writeAttribute(QStringLiteral("count"), QString::number(m_xf_formatsList.size()));
    foreach (const Format &format, m_xf_formatsList) {
        int xf_id = 0;
        writer.writeStartElement(QStringLiteral("xf"));
        if (format.hasNumFmtData())
            writer.writeAttribute(QStringLiteral("numFmtId"), QString::number(format.numberFormatIndex()));
        if (format.hasFontData())
            writer.writeAttribute(QStringLiteral("fontId"), QString::number(format.fontIndex()));
        if (format.hasFillData())
            writer.writeAttribute(QStringLiteral("fillId"), QString::number(format.fillIndex()));
        if (format.hasBorderData())
            writer.writeAttribute(QStringLiteral("borderId"), QString::number(format.borderIndex()));
        writer.writeAttribute(QStringLiteral("xfId"), QString::number(xf_id));
        if (format.hasNumFmtData())
            writer.writeAttribute(QStringLiteral("applyNumberFormat"), QStringLiteral("1"));
        if (format.hasFontData())
            writer.writeAttribute(QStringLiteral("applyFont"), QStringLiteral("1"));
        if (format.hasBorderData())
            writer.writeAttribute(QStringLiteral("applyBorder"), QStringLiteral("1"));
        if (format.hasFillData())
            writer.writeAttribute(QStringLiteral("applyFill"), QStringLiteral("1"));
        if (format.hasAlignmentData())
            writer.writeAttribute(QStringLiteral("applyAlignment"), QStringLiteral("1"));

        if (format.hasAlignmentData()) {
            writer.writeEmptyElement(QStringLiteral("alignment"));
            if (format.hasProperty(FormatPrivate::P_Alignment_AlignH)) {
                switch (format.horizontalAlignment()) {
                case Format::AlignLeft:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("left"));
                    break;
                case Format::AlignHCenter:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("center"));
                    break;
                case Format::AlignRight:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("right"));
                    break;
                case Format::AlignHFill:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("fill"));
                    break;
                case Format::AlignHJustify:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("justify"));
                    break;
                case Format::AlignHMerge:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("centerContinuous"));
                    break;
                case Format::AlignHDistributed:
                    writer.writeAttribute(QStringLiteral("horizontal"), QStringLiteral("distributed"));
                    break;
                default:
                    break;
                }
            }

            if (format.hasProperty(FormatPrivate::P_Alignment_AlignV)) {
                switch (format.verticalAlignment()) {
                case Format::AlignTop:
                    writer.writeAttribute(QStringLiteral("vertical"), QStringLiteral("top"));
                    break;
                case Format::AlignVCenter:
                    writer.writeAttribute(QStringLiteral("vertical"), QStringLiteral("center"));
                    break;
                case Format::AlignVJustify:
                    writer.writeAttribute(QStringLiteral("vertical"), QStringLiteral("justify"));
                    break;
                case Format::AlignVDistributed:
                    writer.writeAttribute(QStringLiteral("vertical"), QStringLiteral("distributed"));
                    break;
                default:
                    break;
                }
            }
            if (format.hasProperty(FormatPrivate::P_Alignment_Indent))
                writer.writeAttribute(QStringLiteral("indent"), QString::number(format.indent()));
            if (format.hasProperty(FormatPrivate::P_Alignment_Wrap) && format.textWrap())
                writer.writeAttribute(QStringLiteral("wrapText"), QStringLiteral("1"));
            if (format.hasProperty(FormatPrivate::P_Alignment_ShinkToFit) && format.shrinkToFit())
                writer.writeAttribute(QStringLiteral("shrinkToFit"), QStringLiteral("1"));
            if (format.hasProperty(FormatPrivate::P_Alignment_Rotation))
                writer.writeAttribute(QStringLiteral("textRotation"), QString::number(format.rotation()));
        }

        writer.writeEndElement();//xf
    }
    writer.writeEndElement();//cellXfs
}

void Styles::writeDxfs(XmlStreamWriter &writer)
{
    writer.writeStartElement(QStringLiteral("dxfs"));
    writer.writeAttribute(QStringLiteral("count"), QString::number(m_dxf_formatsList.size()));
    foreach (const Format &format, m_dxf_formatsList) {
        writer.writeStartElement(QStringLiteral("dxf"));
        Q_UNUSED(format)
        writer.writeEndElement();//dxf
    }
    writer.writeEndElement(); //dxfs
}

bool Styles::readNumFmts(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("numFmts"));
    QXmlStreamAttributes attributes = reader.attributes();
    int count = attributes.value(QLatin1String("count")).toString().toInt();
    for (int i=0; i<count; ++i) {
        reader.readNextStartElement();
        if (reader.name() != QLatin1String("numFmt"))
            return false;
        QXmlStreamAttributes attributes = reader.attributes();
        QSharedPointer<XlsxFormatNumberData> fmt (new XlsxFormatNumberData);
        fmt->formatIndex = attributes.value(QLatin1String("numFmtId")).toString().toInt();
        fmt->formatString = attributes.value(QLatin1String("formatCode")).toString();
        if (fmt->formatIndex >= m_nextCustomNumFmtId)
            m_nextCustomNumFmtId = fmt->formatIndex + 1;
        m_customNumFmtIdMap.insert(fmt->formatIndex, fmt);
        m_customNumFmtsHash.insert(fmt->formatString, fmt);

        while (!(reader.name() == QLatin1String("numFmt") && reader.tokenType() == QXmlStreamReader::EndElement))
            reader.readNextStartElement();
    }
    return true;
}

bool Styles::readFonts(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("fonts"));
    QXmlStreamAttributes attrs = reader.attributes();
    int count = attrs.value(QLatin1String("count")).toString().toInt();
    for (int i=0; i<count; ++i) {
        reader.readNextStartElement();
        if (reader.name() != QLatin1String("font"))
            return false;
        Format format;
        while((reader.readNextStartElement(),true)) { //read until font endelement.
            if (reader.tokenType() == QXmlStreamReader::StartElement) {
                QXmlStreamAttributes attributes = reader.attributes();
                if (reader.name() == QLatin1String("name")) {
                    format.setFontName(attributes.value(QLatin1String("val")).toString());
                } else if (reader.name() == QLatin1String("charset")) {
                    format.setProperty(FormatPrivate::P_Font_Charset, attributes.value(QLatin1String("val")).toString().toInt());
                } else if (reader.name() == QLatin1String("family")) {
                    format.setProperty(FormatPrivate::P_Font_Family, attributes.value(QLatin1String("val")).toString().toInt());
                } else if (reader.name() == QLatin1String("b")) {
                    format.setFontBold(true);
                } else if (reader.name() == QLatin1String("i")) {
                    format.setFontItalic(true);
                } else if (reader.name() == QLatin1String("strike")) {
                    format.setFontStrikeOut(true);
                } else if (reader.name() == QLatin1String("outline")) {
                    format.setFontOutline(true);
                } else if (reader.name() == QLatin1String("shadow")) {
                    format.setProperty(FormatPrivate::P_Font_Shadow, true);
                } else if (reader.name() == QLatin1String("condense")) {
                    format.setProperty(FormatPrivate::P_Font_Condense, attributes.value(QLatin1String("val")).toString().toInt());
                } else if (reader.name() == QLatin1String("extend")) {
                    format.setProperty(FormatPrivate::P_Font_Extend, attributes.value(QLatin1String("val")).toString().toInt());
                } else if (reader.name() == QLatin1String("color")) {
                    if (attributes.hasAttribute(QLatin1String("rgb"))) {
                        QString colorString = attributes.value(QLatin1String("rgb")).toString();
                        format.setFontColor(fromARGBString(colorString));
                    } else if (attributes.hasAttribute(QLatin1String("indexed"))) {
                        QColor color = getColorByIndex(attributes.value(QLatin1String("indexed")).toString().toInt());
                        format.setFontColor(color);
                    } else if (attributes.hasAttribute(QLatin1String("theme"))) {
                        QString theme = attributes.value(QLatin1String("theme")).toString();
                        QString tint = attributes.value(QLatin1String("tint")).toString();
                        format.setProperty(FormatPrivate::P_Font_ThemeColor, QString(theme + QLatin1Char(':') + tint));
                    }
                } else if (reader.name() == QLatin1String("sz")) {
                    int sz = attributes.value(QLatin1String("val")).toString().toInt();
                    format.setFontSize(sz);
                } else if (reader.name() == QLatin1String("u")) {
                    QString value = attributes.value(QLatin1String("val")).toString();
                    if (value == QLatin1String("double"))
                        format.setFontUnderline(Format::FontUnderlineDouble);
                    else if (value == QLatin1String("doubleAccounting"))
                        format.setFontUnderline(Format::FontUnderlineDoubleAccounting);
                    else if (value == QLatin1String("singleAccounting"))
                        format.setFontUnderline(Format::FontUnderlineSingleAccounting);
                    else
                        format.setFontUnderline(Format::FontUnderlineSingle);
                } else if (reader.name() == QLatin1String("vertAlign")) {
                    QString value = attributes.value(QLatin1String("val")).toString();
                    if (value == QLatin1String("superscript"))
                        format.setFontScript(Format::FontScriptSuper);
                    else if (value == QLatin1String("subscript"))
                        format.setFontScript(Format::FontScriptSub);
                } else if (reader.name() == QLatin1String("scheme")) {
                    format.setProperty(FormatPrivate::P_Font_Scheme, attributes.value(QLatin1String("val")).toString());
                }
            }

            if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == QLatin1String("font"))
                break;
        }
        m_fontsList.append(format);
        m_fontsHash.insert(format.fontKey(), format);
        if (format.isValid())
            format.setFontIndex(m_fontsList.size()-1);
    }
    return true;
}

bool Styles::readFills(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("fills"));

    QXmlStreamAttributes attributes = reader.attributes();
    int count = attributes.value(QLatin1String("count")).toString().toInt();
    for (int i=0; i<count; ++i) {
        reader.readNextStartElement();
        if (reader.name() != QLatin1String("fill") || reader.tokenType() != QXmlStreamReader::StartElement)
            return false;
        readFill(reader);
    }
    return true;
}

bool Styles::readFill(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("fill"));

    static QMap<QString, Format::FillPattern> patternValues;
    if (patternValues.isEmpty()) {
        patternValues[QStringLiteral("none")] = Format::PatternNone;
        patternValues[QStringLiteral("solid")] = Format::PatternSolid;
        patternValues[QStringLiteral("mediumGray")] = Format::PatternMediumGray;
        patternValues[QStringLiteral("darkGray")] = Format::PatternDarkGray;
        patternValues[QStringLiteral("lightGray")] = Format::PatternLightGray;
        patternValues[QStringLiteral("darkHorizontal")] = Format::PatternDarkHorizontal;
        patternValues[QStringLiteral("darkVertical")] = Format::PatternDarkVertical;
        patternValues[QStringLiteral("darkDown")] = Format::PatternDarkDown;
        patternValues[QStringLiteral("darkUp")] = Format::PatternDarkUp;
        patternValues[QStringLiteral("darkGrid")] = Format::PatternDarkGrid;
        patternValues[QStringLiteral("darkTrellis")] = Format::PatternDarkTrellis;
        patternValues[QStringLiteral("lightHorizontal")] = Format::PatternLightHorizontal;
        patternValues[QStringLiteral("lightVertical")] = Format::PatternLightVertical;
        patternValues[QStringLiteral("lightDown")] = Format::PatternLightDown;
        patternValues[QStringLiteral("lightUp")] = Format::PatternLightUp;
        patternValues[QStringLiteral("lightTrellis")] = Format::PatternLightTrellis;
        patternValues[QStringLiteral("gray125")] = Format::PatternGray125;
        patternValues[QStringLiteral("gray0625")] = Format::PatternGray0625;
        patternValues[QStringLiteral("lightGrid")] = Format::PatternLightGrid;
    }

    Format fill;
    while((reader.readNextStartElement(), true)) { //read until fill endelement
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("patternFill")) {
                QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute(QLatin1String("patternType"))) {
                    QString pattern = attributes.value(QLatin1String("patternType")).toString();
                    fill.setFillPattern(patternValues.contains(pattern) ? patternValues[pattern] : Format::PatternNone);
                }
            } else if (reader.name() == QLatin1String("fgColor")) {
                QXmlStreamAttributes attributes = reader.attributes();
                QColor c;
                if (attributes.hasAttribute(QLatin1String("rgb"))) {
                    c = fromARGBString(attributes.value(QLatin1String("rgb")).toString());
                } else if (attributes.hasAttribute(QLatin1String("indexed"))) {
                    c = getColorByIndex(attributes.value(QLatin1String("indexed")).toString().toInt());
                } else if (attributes.hasAttribute(QLatin1String("theme"))) {
                    QString theme = attributes.value(QLatin1String("theme")).toString();
                    QString tint = attributes.value(QLatin1String("tint")).toString();
                    fill.setProperty(FormatPrivate::P_Fill_FgThemeColor, QString(theme + QLatin1Char(':') + tint));
                }
                if (fill.fillPattern() == Format::PatternSolid)
                    fill.setPatternBackgroundColor(c);
                else
                    fill.setPatternForegroundColor(c);
            } else if (reader.name() == QLatin1String("bgColor")) {
                QXmlStreamAttributes attributes = reader.attributes();
                QColor c;
                if (attributes.hasAttribute(QLatin1String("rgb"))) {
                    c = fromARGBString(attributes.value(QLatin1String("rgb")).toString());
                } else if (attributes.hasAttribute(QLatin1String("indexed"))) {
                    c = getColorByIndex(attributes.value(QLatin1String("indexed")).toString().toInt());
                } else if (attributes.hasAttribute(QLatin1String("theme"))) {
                    QString theme = attributes.value(QLatin1String("theme")).toString();
                    QString tint = attributes.value(QLatin1String("tint")).toString();
                    fill.setProperty(FormatPrivate::P_Fill_BgThemeColor, QString(theme + QLatin1Char(':') + tint));
                }
                if (fill.fillPattern() == Format::PatternSolid)
                    fill.setPatternForegroundColor(c);
                else
                    fill.setPatternBackgroundColor(c);
            }
        }

        if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == QLatin1String("fill"))
            break;
    }

    m_fillsList.append(fill);
    m_fillsHash.insert(fill.fillKey(), fill);
    if (fill.isValid())
        fill.setFillIndex(m_fillsList.size()-1);

    return true;
}

bool Styles::readBorders(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("borders"));

    QXmlStreamAttributes attributes = reader.attributes();
    int count = attributes.value(QLatin1String("count")).toString().toInt();
    for (int i=0; i<count; ++i) {
        reader.readNextStartElement();
        if (reader.name() != QLatin1String("border") || reader.tokenType() != QXmlStreamReader::StartElement)
            return false;
        readBorder(reader);
    }
    return true;
}

bool Styles::readBorder(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("border"));
    Format border;

    QXmlStreamAttributes attributes = reader.attributes();
    bool isUp = attributes.hasAttribute(QLatin1String("diagonalUp"));
    bool isDown = attributes.hasAttribute(QLatin1String("diagonalUp"));
    if (isUp && isDown)
        border.setDiagonalBorderType(Format::DiagnoalBorderBoth);
    else if (isUp)
        border.setDiagonalBorderType(Format::DiagonalBorderUp);
    else if (isDown)
        border.setDiagonalBorderType(Format::DiagonalBorderDown);

    while((reader.readNextStartElement(), true)) { //read until border endelement
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("left") || reader.name() == QLatin1String("right")
                    || reader.name() == QLatin1String("top") || reader.name() == QLatin1String("bottom")
                    || reader.name() == QLatin1String("diagonal") ) {
                Format::BorderStyle style(Format::BorderNone);
                QColor color;
                QString themeColor;
                readSubBorder(reader, reader.name().toString(), style, color, themeColor);

                if (reader.name() == QLatin1String("left")) {
                    border.setLeftBorderStyle(style);
                    if (color.isValid())
                        border.setLeftBorderColor(color);
                    else if (!themeColor.isEmpty())
                        border.setProperty(FormatPrivate::P_Border_ThemeLeftColor, themeColor);
                } else if (reader.name() == QLatin1String("right")) {
                    border.setRightBorderStyle(style);
                    if (color.isValid())
                        border.setRightBorderColor(color);
                    else if (!themeColor.isEmpty())
                        border.setProperty(FormatPrivate::P_Border_ThemeRightColor, themeColor);
                } else if (reader.name() == QLatin1String("top")) {
                    border.setTopBorderStyle(style);
                    if (color.isValid())
                        border.setTopBorderColor(color);
                    else if (!themeColor.isEmpty())
                        border.setProperty(FormatPrivate::P_Border_ThemeTopColor, themeColor);
                } else if (reader.name() == QLatin1String("bottom")) {
                    border.setBottomBorderStyle(style);
                    if (color.isValid())
                        border.setBottomBorderColor(color);
                    else if (!themeColor.isEmpty())
                        border.setProperty(FormatPrivate::P_Border_ThemeBottomColor, themeColor);
                } else if (reader.name() == QLatin1String("diagonal")) {
                    border.setDiagonalBorderStyle(style);
                    if (color.isValid())
                        border.setDiagonalBorderColor(color);
                    else if (!themeColor.isEmpty())
                        border.setProperty(FormatPrivate::P_Border_ThemeDiagonalColor, themeColor);
                }
            }
        }

        if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == QLatin1String("border"))
            break;
    }

    m_bordersList.append(border);
    m_bordersHash.insert(border.borderKey(), border);
    if (border.isValid())
        border.setBorderIndex(m_bordersList.size()-1);

    return true;
}

bool Styles::readSubBorder(XmlStreamReader &reader, const QString &name, Format::BorderStyle &style, QColor &color, QString &themeColor)
{
    Q_ASSERT(reader.name() == name);

    static QMap<QString, Format::BorderStyle> stylesStringsMap;
    if (stylesStringsMap.isEmpty()) {
        stylesStringsMap[QStringLiteral("none")] = Format::BorderNone;
        stylesStringsMap[QStringLiteral("thin")] = Format::BorderThin;
        stylesStringsMap[QStringLiteral("medium")] = Format::BorderMedium;
        stylesStringsMap[QStringLiteral("dashed")] = Format::BorderDashed;
        stylesStringsMap[QStringLiteral("dotted")] = Format::BorderDotted;
        stylesStringsMap[QStringLiteral("thick")] = Format::BorderThick;
        stylesStringsMap[QStringLiteral("double")] = Format::BorderDouble;
        stylesStringsMap[QStringLiteral("hair")] = Format::BorderHair;
        stylesStringsMap[QStringLiteral("mediumDashed")] = Format::BorderMediumDashed;
        stylesStringsMap[QStringLiteral("dashDot")] = Format::BorderDashDot;
        stylesStringsMap[QStringLiteral("mediumDashDot")] = Format::BorderMediumDashDot;
        stylesStringsMap[QStringLiteral("dashDotDot")] = Format::BorderDashDotDot;
        stylesStringsMap[QStringLiteral("mediumDashDotDot")] = Format::BorderMediumDashDotDot;
        stylesStringsMap[QStringLiteral("slantDashDot")] = Format::BorderSlantDashDot;
    }

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute(QLatin1String("style"))) {
        QString styleString = attributes.value(QLatin1String("style")).toString();
        if (stylesStringsMap.contains(styleString)) {
            //get style
            style = stylesStringsMap[styleString];
            while((reader.readNextStartElement(),true)) {
                if (reader.tokenType() == QXmlStreamReader::StartElement) {
                    if (reader.name() == QLatin1String("color")) {
                        QXmlStreamAttributes colorAttrs = reader.attributes();
                        if (colorAttrs.hasAttribute(QLatin1String("rgb"))) {
                            QString colorString = colorAttrs.value(QLatin1String("rgb")).toString();
                            //get color
                            color = fromARGBString(colorString);
                        } else if (colorAttrs.hasAttribute(QLatin1String("indexed"))) {
                            color = getColorByIndex(colorAttrs.value(QLatin1String("indexed")).toString().toInt());
                        } else if (colorAttrs.hasAttribute(QLatin1String("theme"))) {
                            QString theme = attributes.value(QLatin1String("theme")).toString();
                            QString tint = attributes.value(QLatin1String("tint")).toString();
                            themeColor = theme + QLatin1Char(':') + tint;
                        }
                    }

                } else if (reader.tokenType() == QXmlStreamReader::EndElement) {
                    if (reader.name() == name)
                        break;
                }
            }
        }
    }

    return true;
}

bool Styles::readCellXfs(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("cellXfs"));
    QXmlStreamAttributes attributes = reader.attributes();
    int count = attributes.value(QLatin1String("count")).toString().toInt();
    for (int i=0; i<count; ++i) {
        reader.readNextStartElement();
        if (reader.name() != QLatin1String("xf"))
            return false;
        Format format;
        QXmlStreamAttributes xfAttrs = reader.attributes();

//        qDebug()<<reader.name()<<reader.tokenString()<<" .........";
//        for (int i=0; i<xfAttrs.size(); ++i)
//            qDebug()<<"... "<<i<<" "<<xfAttrs[i].name()<<xfAttrs[i].value();

        if (xfAttrs.hasAttribute(QLatin1String("applyNumberFormat"))) {
            int numFmtIndex = xfAttrs.value(QLatin1String("numFmtId")).toString().toInt();
            if (!m_customNumFmtIdMap.contains(numFmtIndex))
                format.setNumberFormatIndex(numFmtIndex);
            else
                format.setNumberFormat(numFmtIndex, m_customNumFmtIdMap[numFmtIndex]->formatString);
        }

        if (xfAttrs.hasAttribute(QLatin1String("applyFont"))) {
            int fontIndex = xfAttrs.value(QLatin1String("fontId")).toString().toInt();
            if (fontIndex >= m_fontsList.size()) {
                qDebug("Error read styles.xml, cellXfs fontId");
            } else {
                Format fontFormat = m_fontsList[fontIndex];
                for (int i=FormatPrivate::P_Font_STARTID; i<FormatPrivate::P_Font_ENDID; ++i) {
                    if (fontFormat.hasProperty(i))
                        format.setProperty(i, fontFormat.property(i));
                }
            }
        }

        if (xfAttrs.hasAttribute(QLatin1String("applyFill"))) {
            int id = xfAttrs.value(QLatin1String("fillId")).toString().toInt();
            if (id >= m_fillsList.size()) {
                qDebug("Error read styles.xml, cellXfs fillId");
            } else {
                Format fillFormat = m_fillsList[id];
                for (int i=FormatPrivate::P_Fill_STARTID; i<FormatPrivate::P_Fill_ENDID; ++i) {
                    if (fillFormat.hasProperty(i))
                        format.setProperty(i, fillFormat.property(i));
                }
            }
        }

        if (xfAttrs.hasAttribute(QLatin1String("applyBorder"))) {
            int id = xfAttrs.value(QLatin1String("borderId")).toString().toInt();
            if (id >= m_bordersList.size()) {
                qDebug("Error read styles.xml, cellXfs borderId");
            } else {
                Format borderFormat = m_bordersList[id];
                for (int i=FormatPrivate::P_Border_STARTID; i<FormatPrivate::P_Border_ENDID; ++i) {
                    if (borderFormat.hasProperty(i))
                        format.setProperty(i, borderFormat.property(i));
                }
            }
        }

        if (xfAttrs.hasAttribute(QLatin1String("applyAlignment"))) {
            reader.readNextStartElement();
            if (reader.name() == QLatin1String("alignment")) {
                QXmlStreamAttributes alignAttrs = reader.attributes();

                if (alignAttrs.hasAttribute(QLatin1String("horizontal"))) {
                    static QMap<QString, Format::HorizontalAlignment> alignStringMap;
                    if (alignStringMap.isEmpty()) {
                        alignStringMap.insert(QStringLiteral("left"), Format::AlignLeft);
                        alignStringMap.insert(QStringLiteral("center"), Format::AlignHCenter);
                        alignStringMap.insert(QStringLiteral("right"), Format::AlignRight);
                        alignStringMap.insert(QStringLiteral("justify"), Format::AlignHJustify);
                        alignStringMap.insert(QStringLiteral("centerContinuous"), Format::AlignHMerge);
                        alignStringMap.insert(QStringLiteral("distributed"), Format::AlignHDistributed);
                    }
                    QString str = alignAttrs.value(QLatin1String("horizontal")).toString();
                    if (alignStringMap.contains(str))
                        format.setHorizontalAlignment(alignStringMap[str]);
                }

                if (alignAttrs.hasAttribute(QLatin1String("vertical"))) {
                    static QMap<QString, Format::VerticalAlignment> alignStringMap;
                    if (alignStringMap.isEmpty()) {
                        alignStringMap.insert(QStringLiteral("top"), Format::AlignTop);
                        alignStringMap.insert(QStringLiteral("center"), Format::AlignVCenter);
                        alignStringMap.insert(QStringLiteral("justify"), Format::AlignVJustify);
                        alignStringMap.insert(QStringLiteral("distributed"), Format::AlignVDistributed);
                    }
                    QString str = alignAttrs.value(QLatin1String("vertical")).toString();
                    if (alignStringMap.contains(str))
                        format.setVerticalAlignment(alignStringMap[str]);
                }

                if (alignAttrs.hasAttribute(QLatin1String("indent"))) {
                    int indent = alignAttrs.value(QLatin1String("indent")).toString().toInt();
                    format.setIndent(indent);
                }

                if (alignAttrs.hasAttribute(QLatin1String("textRotation"))) {
                    int rotation = alignAttrs.value(QLatin1String("textRotation")).toString().toInt();
                    format.setRotation(rotation);
                }

                if (alignAttrs.hasAttribute(QLatin1String("wrapText")))
                    format.setTextWarp(true);

                if (alignAttrs.hasAttribute(QLatin1String("shrinkToFit")))
                    format.setShrinkToFit(true);
            }
        }

        addFormat(format, true);

        //Find the endElement of xf
        while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == QLatin1String("xf")))
            reader.readNextStartElement();
    }

    return true;
}

bool Styles::readColors(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("colors"));
    while (!(reader.name() == QLatin1String("colors") && reader.tokenType() == QXmlStreamReader::EndElement)) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("indexedColors")) {
                readIndexedColors(reader);
            } else if (reader.name() == QLatin1String("mruColors")) {

            }
        }
    }
    return true;
}

bool Styles::readIndexedColors(XmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("indexedColors"));
    while (!(reader.name() == QLatin1String("indexedColors") && reader.tokenType() == QXmlStreamReader::EndElement)) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("rgbColor")) {
                QString color = reader.attributes().value(QLatin1String("rgb")).toString();
                m_indexedColors.append(fromARGBString(color));
            }
        }
    }
    return true;
}

bool Styles::loadFromXmlFile(QIODevice *device)
{
    {
        //Try load colors part first!
        XmlStreamReader reader(device);
        while(!reader.atEnd()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == QLatin1String("colors")) {
                    readColors(reader);
                }
            }
        }
        device->seek(0);
    }

    XmlStreamReader reader(device);
    while(!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("numFmts")) {
                readNumFmts(reader);
            } else if (reader.name() == QLatin1String("fonts")) {
                readFonts(reader);
            } else if (reader.name() == QLatin1String("fills")) {
                readFills(reader);
            } else if (reader.name() == QLatin1String("borders")) {
                readBorders(reader);
            } else if (reader.name() == QLatin1String("cellStyleXfs")) {

            } else if (reader.name() == QLatin1String("cellXfs")) {
                readCellXfs(reader);
            } else if (reader.name() == QLatin1String("cellStyles")) {

            }
        }

        if (reader.hasError()) {
            qDebug()<<"Error when read style file: "<<reader.errorString();
        }
    }
    return true;
}

bool Styles::loadFromXmlData(const QByteArray &data)
{
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QIODevice::ReadOnly);

    return loadFromXmlFile(&buffer);
}

QColor Styles::getColorByIndex(int idx)
{
    if (m_indexedColors.isEmpty()) {
        m_indexedColors<<QColor("#000000") <<QColor("#FFFFFF") <<QColor("#FF0000") <<QColor("#00FF00")
                      <<QColor("#0000FF") <<QColor("#FFFF00") <<QColor("#FF00FF") <<QColor("#00FFFF")
                     <<QColor("#000000") <<QColor("#FFFFFF") <<QColor("#FF0000") <<QColor("#00FF00")
                    <<QColor("#0000FF") <<QColor("#FFFF00") <<QColor("#FF00FF") <<QColor("#00FFFF")
                   <<QColor("#800000") <<QColor("#008000") <<QColor("#000080") <<QColor("#808000")
                  <<QColor("#800080") <<QColor("#008080") <<QColor("#C0C0C0") <<QColor("#808080")
                 <<QColor("#9999FF") <<QColor("#993366") <<QColor("#FFFFCC") <<QColor("#CCFFFF")
                <<QColor("#660066") <<QColor("#FF8080") <<QColor("#0066CC") <<QColor("#CCCCFF")
               <<QColor("#000080") <<QColor("#FF00FF") <<QColor("#FFFF00") <<QColor("#00FFFF")
              <<QColor("#800080") <<QColor("#800000") <<QColor("#008080") <<QColor("#0000FF")
             <<QColor("#00CCFF") <<QColor("#CCFFFF") <<QColor("#CCFFCC") <<QColor("#FFFF99")
            <<QColor("#99CCFF") <<QColor("#FF99CC") <<QColor("#CC99FF") <<QColor("#FFCC99")
           <<QColor("#3366FF") <<QColor("#33CCCC") <<QColor("#99CC00") <<QColor("#FFCC00")
          <<QColor("#FF9900") <<QColor("#FF6600") <<QColor("#666699") <<QColor("#969696")
         <<QColor("#003366") <<QColor("#339966") <<QColor("#003300") <<QColor("#333300")
        <<QColor("#993300") <<QColor("#993366") <<QColor("#333399") <<QColor("#333333");
    }
    if (idx < 0 || idx >= m_indexedColors.size())
        return QColor();
    return m_indexedColors[idx];
}

} //namespace QXlsx
