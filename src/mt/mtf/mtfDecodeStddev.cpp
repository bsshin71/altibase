/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 

/***********************************************************************
 * $Id: mtfDecodeStddev.cpp 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#include <mte.h>
#include <mtc.h>
#include <mtd.h>
#include <mtf.h>
#include <mtk.h>
#include <mtv.h>

#include <mtdTypes.h>

extern mtfModule mtfDecodeStddev;

extern mtdModule mtdDouble;
extern mtdModule mtdList;
extern mtdModule mtdBoolean;
extern mtdModule mtdBinary;

static mtcName mtfDecodeStddevFunctionName[1] = {
    { NULL, 13, (void*)"DECODE_STDDEV" }
};

static IDE_RC mtfDecodeStddevEstimate( mtcNode*     aNode,
                                       mtcTemplate* aTemplate,
                                       mtcStack*    aStack,
                                       SInt         aRemain,
                                       mtcCallBack* aCallBack );

mtfModule mtfDecodeStddev = {
    2|MTC_NODE_OPERATOR_AGGREGATION,
    ~(MTC_NODE_INDEX_MASK),
    1.0,  // default selectivity (�� �����ڰ� �ƴ�)
    mtfDecodeStddevFunctionName,
    NULL,
    mtf::initializeDefault,
    mtf::finalizeDefault,
    mtfDecodeStddevEstimate
};

IDE_RC mtfDecodeStddevInitialize( mtcNode*     aNode,
                                  mtcStack*    aStack,
                                  SInt         aRemain,
                                  void*        aInfo,
                                  mtcTemplate* aTemplate );

IDE_RC mtfDecodeStddevAggregate( mtcNode*     aNode,
                                 mtcStack*    aStack,
                                 SInt         aRemain,
                                 void*        aInfo,
                                 mtcTemplate* aTemplate );

IDE_RC mtfDecodeStddevFinalize( mtcNode*     aNode,
                                mtcStack*    aStack,
                                SInt         aRemain,
                                void*        aInfo,
                                mtcTemplate* aTemplate );

IDE_RC mtfDecodeStddevCalculate( mtcNode*     aNode,
                                 mtcStack*    aStack,
                                 SInt         aRemain,
                                 void*        aInfo,
                                 mtcTemplate* aTemplate );

static const mtcExecute mtfDecodeStddevExecute = {
    mtfDecodeStddevInitialize,
    mtfDecodeStddevAggregate,
    mtf::calculateNA,
    mtfDecodeStddevFinalize,
    mtfDecodeStddevCalculate,
    NULL,
    mtk::estimateRangeNA,
    mtk::extractRangeNA
};

typedef struct mtfDecodeStddevInfo
{
    // ù��° ����
    mtcExecute   * sStddevColumnExecute;
    mtcNode      * sStddevColumnNode;

    // �ι�° ����
    mtcExecute   * sExprExecute;
    mtcNode      * sExprNode;

    // ����° ����
    mtcExecute   * sSearchExecute;
    mtcNode      * sSearchNode;

    // return ����
    mtcColumn    * sReturnColumn;
    void         * sReturnValue;

    // �ӽú���
    mtdDoubleType  sPow;
    mtdDoubleType  sSum;
    ULong          sCount;
} mtfDecodeStddevInfo;

IDE_RC mtfDecodeStddevEstimate( mtcNode*     aNode,
                                mtcTemplate* aTemplate,
                                mtcStack*    aStack,
                                SInt      /* aRemain */,
                                mtcCallBack* aCallBack )
{
#define IDE_FN "IDE_RC mtfDecodeStddevEstimate"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    const mtdModule * sTarget;
    const mtdModule * sModules[MTC_NODE_ARGUMENT_COUNT_MAXIMUM];
    mtcStack        * sStack1;
    mtcStack        * sStack2;
    UInt              sBinaryPrecision;
    UInt              sCount;
    UInt              sFence;

    sFence = aNode->lflag & MTC_NODE_ARGUMENT_COUNT_MASK;

    // 1 Ȥ�� 3���� ����
    IDE_TEST_RAISE( (sFence != 1) && (sFence != 3),
                    ERR_INVALID_FUNCTION_ARGUMENT );

    aStack[0].column = aTemplate->rows[aNode->table].columns + aNode->column;

    if ( sFence == 3 )
    {
        if ( aStack[2].column->module != &mtdList )
        {
            IDE_TEST( mtf::getComparisonModule(
                          &sTarget,
                          aStack[2].column->module->no,
                          aStack[3].column->module->no )
                      != IDE_SUCCESS );
            IDE_TEST_RAISE( sTarget == NULL,
                            ERR_CONVERSION_NOT_APPLICABLE );

            // To Fix PR-15208
            IDE_TEST_RAISE( mtf::isEquiValidType( sTarget ) != ID_TRUE,
                            ERR_CONVERSION_NOT_APPLICABLE );

            sModules[0] = & mtdDouble;
            sModules[1] = sTarget;
            sModules[2] = sTarget;

            IDE_TEST( mtf::makeConversionNodes( aNode,
                                                aNode->arguments,
                                                aTemplate,
                                                aStack + 1,
                                                aCallBack,
                                                sModules )
                      != IDE_SUCCESS );
        }
        else
        {
            IDE_TEST_RAISE( aStack[3].column->module != &mtdList,
                            ERR_CONVERSION_NOT_APPLICABLE );
            IDE_TEST_RAISE( (aStack[2].column->precision !=
                             aStack[3].column->precision) ||
                            (aStack[2].column->precision <= 0),
                            ERR_INVALID_FUNCTION_ARGUMENT );

            sStack1 = (mtcStack*)aStack[2].value;
            sStack2 = (mtcStack*)aStack[3].value;

            for( sCount = 0, sFence = aStack[2].column->precision;
                 sCount < sFence;
                 sCount++ )
            {
                IDE_TEST( mtf::getComparisonModule(
                              &sTarget,
                              sStack1[sCount].column->module->no,
                              sStack2[sCount].column->module->no )
                          != IDE_SUCCESS );

                IDE_TEST_RAISE( (sTarget == NULL) || (sTarget == &mtdList),
                                ERR_CONVERSION_NOT_APPLICABLE );

                // To Fix PR-15208
                IDE_TEST_RAISE( mtf::isEquiValidType( sTarget ) != ID_TRUE,
                                ERR_CONVERSION_NOT_APPLICABLE );
                sModules[sCount] = sTarget;
            }

            IDE_TEST( mtf::makeConversionNodes( aNode,
                                                aNode->arguments->next->arguments,
                                                aTemplate,
                                                sStack1,
                                                aCallBack,
                                                sModules )
                      != IDE_SUCCESS );
            IDE_TEST( mtf::makeConversionNodes( aNode,
                                                aNode->arguments->next->next->arguments,
                                                aTemplate,
                                                sStack2,
                                                aCallBack,
                                                sModules )
                      != IDE_SUCCESS );

            sModules[0] = & mtdDouble;
            sModules[1] = & mtdList;
            sModules[2] = & mtdList;

            IDE_TEST( mtf::makeConversionNodes( aNode,
                                                aNode->arguments,
                                                aTemplate,
                                                aStack + 1,
                                                aCallBack,
                                                sModules )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        sModules[0] = & mtdDouble;
        IDE_TEST( mtf::makeConversionNodes( aNode,
                                            aNode->arguments,
                                            aTemplate,
                                            aStack + 1,
                                            aCallBack,
                                            sModules )
                  != IDE_SUCCESS );
    }

    aTemplate->rows[aNode->table].execute[aNode->column] = mtfDecodeStddevExecute;

    // stddev ����� ������
    IDE_TEST( mtc::initializeColumn( aStack[0].column,
                                     & mtdDouble,
                                     0,
                                     0,
                                     0 )
              != IDE_SUCCESS );

    // stddev info ������ mtdBinary�� ����
    sBinaryPrecision = ID_SIZEOF(mtfDecodeStddevInfo);

    IDE_TEST( mtc::initializeColumn( aStack[0].column + 1,
                                     & mtdBinary,
                                     1,
                                     sBinaryPrecision,
                                     0 )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_FUNCTION_ARGUMENT );
    IDE_SET(ideSetErrorCode(mtERR_ABORT_INVALID_FUNCTION_ARGUMENT));

    IDE_EXCEPTION( ERR_CONVERSION_NOT_APPLICABLE );
    IDE_SET(ideSetErrorCode(mtERR_ABORT_CONVERSION_NOT_APPLICABLE));

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
#undef IDE_FN
}

IDE_RC mtfDecodeStddevInitialize( mtcNode*     aNode,
                                  mtcStack*,
                                  SInt,
                                  void*,
                                  mtcTemplate* aTemplate )
{
#define IDE_FN "IDE_RC mtfDecodeStddevInitialize"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    const mtcColumn   * sColumn;
    mtcNode           * sArgNode[3];
    mtdBinaryType     * sValue;
    mtfDecodeStddevInfo  * sInfo;
    UInt                sFence;

    sFence = aNode->lflag & MTC_NODE_ARGUMENT_COUNT_MASK;

    sColumn = aTemplate->rows[aNode->table].columns + aNode->column;
    sValue = (mtdBinaryType*)
        ((UChar*)aTemplate->rows[aNode->table].row + sColumn[1].column.offset);

    // BUG-43709
    sValue->mLength = sColumn[1].precision;
    sInfo = (mtfDecodeStddevInfo*)(sValue->mValue);

    //-----------------------------
    // stddev info �ʱ�ȭ
    //-----------------------------
    sArgNode[0] = aNode->arguments;

    // stddev column ����
    sInfo->sStddevColumnExecute = aTemplate->rows[sArgNode[0]->table].execute + sArgNode[0]->column;
    sInfo->sStddevColumnNode    = sArgNode[0];

    if ( sFence == 3 )
    {
        sArgNode[1] = sArgNode[0]->next;
        sArgNode[2] = sArgNode[1]->next;

        // expression column ����
        sInfo->sExprExecute = aTemplate->rows[sArgNode[1]->table].execute + sArgNode[1]->column;
        sInfo->sExprNode    = sArgNode[1];

        // search value ����
        sInfo->sSearchExecute = aTemplate->rows[sArgNode[2]->table].execute + sArgNode[2]->column;
        sInfo->sSearchNode    = sArgNode[2];
    }
    else
    {
        sInfo->sExprExecute = NULL;
        sInfo->sExprNode    = NULL;

        sInfo->sSearchExecute = NULL;
        sInfo->sSearchNode    = NULL;
    }

    // return column ����
    sInfo->sReturnColumn = aTemplate->rows[aNode->table].columns + aNode->column;
    sInfo->sReturnValue  = (void *)
        ((UChar*) aTemplate->rows[aNode->table].row + sInfo->sReturnColumn->column.offset);

    // �ӽú��� �ʱ�ȭ
    sInfo->sPow   = 0;
    sInfo->sSum   = 0;
    sInfo->sCount = 0;

    //-----------------------------
    // stddev ����� �ʱ�ȭ
    //-----------------------------

    *(mtdDoubleType*)(sInfo->sReturnValue) = 0;

    return IDE_SUCCESS;

#undef IDE_FN
}

IDE_RC mtfDecodeStddevAggregate( mtcNode*     aNode,
                                 mtcStack*    aStack,
                                 SInt         aRemain,
                                 void*,
                                 mtcTemplate* aTemplate )
{
#define IDE_FN "IDE_RC mtfDecodeStddevAggregate"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    const mtdModule   * sModule;
    const mtcColumn   * sColumn;
    mtdBinaryType     * sValue;
    mtfDecodeStddevInfo  * sInfo;
    mtdValueInfo        sValueInfo1;
    mtdValueInfo        sValueInfo2;
    mtcStack          * sStack1;
    mtcStack          * sStack2;
    SInt                sCompare;
    UInt                sCount;
    UInt                sFence;

    sFence = aNode->lflag & MTC_NODE_ARGUMENT_COUNT_MASK;

    sColumn = aTemplate->rows[aNode->table].columns + aNode->column;
    sValue = (mtdBinaryType*)((UChar*)aTemplate->rows[aNode->table].row
                              + sColumn[1].column.offset);
    sInfo = (mtfDecodeStddevInfo*)(sValue->mValue);

    if ( sFence == 3 )
    {
        IDE_TEST_RAISE( aRemain < 2, ERR_STACK_OVERFLOW );

        // �ι�° ����
        IDE_TEST( sInfo->sExprExecute->calculate( sInfo->sExprNode,
                                                  aStack,
                                                  aRemain,
                                                  sInfo->sExprExecute->calculateInfo,
                                                  aTemplate )
                  != IDE_SUCCESS );

        if( sInfo->sExprNode->conversion != NULL )
        {
            IDE_TEST( mtf::convertCalculate( sInfo->sExprNode,
                                             aStack,
                                             aRemain,
                                             NULL,
                                             aTemplate )
                      != IDE_SUCCESS );
        }

        // ����° ����
        IDE_TEST( sInfo->sSearchExecute->calculate( sInfo->sSearchNode,
                                                    aStack + 1,
                                                    aRemain - 1,
                                                    sInfo->sSearchExecute->calculateInfo,
                                                    aTemplate )
                  != IDE_SUCCESS );

        if( sInfo->sSearchNode->conversion != NULL )
        {
            IDE_TEST( mtf::convertCalculate( sInfo->sSearchNode,
                                             aStack + 1,
                                             aRemain - 1,
                                             NULL,
                                             aTemplate )
                      != IDE_SUCCESS );
        }

        // decode �������
        if ( aStack[0].column->module != &mtdList )
        {
            IDE_DASSERT( aStack[0].column->module == aStack[1].column->module );

            sModule = aStack[0].column->module;

            if ( ( sModule->isNull( aStack[0].column,
                                    aStack[0].value ) == ID_TRUE ) ||
                 ( sModule->isNull( aStack[1].column,
                                    aStack[1].value ) == ID_TRUE ) )
            {
                sCompare = -1;
            }
            else
            {
                sValueInfo1.column = aStack[0].column;
                sValueInfo1.value  = aStack[0].value;
                sValueInfo1.flag   = MTD_OFFSET_USELESS;

                sValueInfo2.column = aStack[1].column;
                sValueInfo2.value  = aStack[1].value;
                sValueInfo2.flag   = MTD_OFFSET_USELESS;

                // �ι�° ���ڿ� ����° ������ ��
                sCompare = sModule->logicalCompare[MTD_COMPARE_ASCENDING]( &sValueInfo1,
                                                                           &sValueInfo2 );
            }
        }
        else
        {
            IDE_DASSERT( aStack[0].column->module == &mtdList );
            IDE_DASSERT( (aStack[0].column->precision ==
                          aStack[1].column->precision) &&
                         (aStack[0].column->precision > 0) );

            sStack1 = (mtcStack*)aStack[0].value;
            sStack2 = (mtcStack*)aStack[1].value;

            for( sCount = 0, sFence = aStack[0].column->precision, sCompare = 0;
                 (sCount < sFence) && (sCompare == 0);
                 sCount++ )
            {
                IDE_DASSERT( sStack1[sCount].column->module == sStack2[sCount].column->module );

                sModule = sStack1[sCount].column->module;

                if ( ( sModule->isNull( sStack1[sCount].column,
                                        sStack1[sCount].value ) == ID_TRUE ) ||
                     ( sModule->isNull( sStack2[sCount].column,
                                        sStack2[sCount].value ) == ID_TRUE ) )
                {
                    sCompare = -1;
                }
                else
                {
                    sValueInfo1.column = sStack1[sCount].column;
                    sValueInfo1.value  = sStack1[sCount].value;
                    sValueInfo1.flag   = MTD_OFFSET_USELESS;

                    sValueInfo2.column = sStack2[sCount].column;
                    sValueInfo2.value  = sStack2[sCount].value;
                    sValueInfo2.flag   = MTD_OFFSET_USELESS;

                    sCompare = sModule->logicalCompare[MTD_COMPARE_ASCENDING]( &sValueInfo1,
                                                                               &sValueInfo2 );
                }
            }
        }
    }
    else
    {
        IDE_TEST_RAISE( aRemain < 1, ERR_STACK_OVERFLOW );
        sCompare = 0;
    }

    // ù��° ����
    IDE_TEST( sInfo->sStddevColumnExecute->calculate( sInfo->sStddevColumnNode,
                                                      aStack,
                                                      aRemain,
                                                      sInfo->sStddevColumnExecute->calculateInfo,
                                                      aTemplate )
              != IDE_SUCCESS );
    if( sInfo->sStddevColumnNode->conversion != NULL )
    {
        IDE_TEST( mtf::convertCalculate( sInfo->sStddevColumnNode,
                                         aStack,
                                         aRemain,
                                         NULL,
                                         aTemplate )
                  != IDE_SUCCESS );
    }

    IDE_DASSERT( aStack[0].column->module == & mtdDouble );

    sModule = aStack[0].column->module;

    if ( sModule->isNull( aStack[0].column,
                          aStack[0].value ) != ID_TRUE )
    {
        if ( sCompare == 0 )
        {
            sInfo->sPow += *(mtdDoubleType*)aStack[0].value
                * *(mtdDoubleType*)aStack[0].value;
            sInfo->sSum += *(mtdDoubleType*)aStack[0].value;
            sInfo->sCount++;
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_STACK_OVERFLOW );
    IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
#undef IDE_FN
}

IDE_RC mtfDecodeStddevFinalize( mtcNode*     aNode,
                                mtcStack*,
                                SInt,
                                void*,
                                mtcTemplate* aTemplate )
{
#define IDE_FN "IDE_RC mtfDecodeStddevFinalize"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    const mtcColumn   * sColumn;
    mtdBinaryType     * sValue;
    mtfDecodeStddevInfo  * sInfo;

    sColumn = aTemplate->rows[aNode->table].columns + aNode->column;
    sValue = (mtdBinaryType*)((UChar*)aTemplate->rows[aNode->table].row
                              + sColumn[1].column.offset);
    sInfo = (mtfDecodeStddevInfo*)(sValue->mValue);

    if( sInfo->sCount == 0 )
    {
        mtdDouble.null( sInfo->sReturnColumn,
                        sInfo->sReturnValue );
    }
    else
    {
        if ( sInfo->sCount == 1 )
        {
            *(mtdDoubleType*)(sInfo->sReturnValue) = 0;
        }
        else
        {
            *(mtdDoubleType*)(sInfo->sReturnValue) = idlOS::sqrt(
                idlOS::fabs( ( sInfo->sPow - sInfo->sSum * sInfo->sSum / sInfo->sCount )
                             / ( sInfo->sCount - 1 ) ) );
        }
    }

    return IDE_SUCCESS;

#undef IDE_FN
}

IDE_RC mtfDecodeStddevCalculate( mtcNode*     aNode,
                                 mtcStack*    aStack,
                                 SInt,
                                 void*,
                                 mtcTemplate* aTemplate )
{
#define IDE_FN "IDE_RC mtfDecodeStddevCalculate"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    aStack->column = aTemplate->rows[aNode->table].columns + aNode->column;
    aStack->value  = (void*)( (UChar*) aTemplate->rows[aNode->table].row
                              + aStack->column->column.offset );
    return IDE_SUCCESS;

#undef IDE_FN
}